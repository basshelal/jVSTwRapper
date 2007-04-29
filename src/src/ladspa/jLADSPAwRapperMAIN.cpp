
#include "ladspa.h"
#include "JNIUtils.h"
#include "jVSTwRapperMAIN.h"

#include <aeffect.h>
#include <aeffectx.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//***********************************************************************************
// VST AudioMaster callback
//

typedef AEffect *( *vstPluginMain )( audioMasterCallback audioMaster );

extern "C" {
   static VstIntPtr audioMaster( AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
      switch (opcode) {
         case audioMasterVersion:
            return 2;

         case audioMasterCurrentId:
            return 1;
      }
      return 0;
   }
}


//***********************************************************************************
// This class models a PLUGIN
//

class Plug {
 public:
   Plug();
   ~Plug();

   bool Init( unsigned long id, char *name, AEffect *effect, AEffect *effectPtr );
   char *GetStr( VstInt32 opcode, char *dflt );
   Plug *GetPrev();
   void SetPrev( Plug *prev );
   LADSPA_Descriptor *GetDescriptor();
   LADSPA_Handle Instantiate( unsigned long rate );
   
 private:
   Plug *mPrev;
   AEffect *mThunk;
   AEffect *mEffect;
   char *mName;
   void *mLib;
   LADSPA_Descriptor mDesc;
};


//***********************************************************************************
// This class models a PLUGIN instance
//

class Instance {
 public:
   Instance();
   ~Instance();
 
   bool Init( char *name, unsigned long rate, AEffect *peffect, AEffect* pthunk);
   void Cleanup();

   void Connect_Port( unsigned long port, LADSPA_Data *data );
   void Run( unsigned long count );
   void Run_Adding( unsigned long count );
   void Set_Run_Adding_Gain( LADSPA_Data gain );
   void Set_Parameters();
   
 private:
   AEffect *mThunk;
   AEffect *mEffect;
   LADSPA_Data **mPorts;
   LADSPA_Data mGain;
   unsigned long mBlockSize;
   int mNumPorts;
   int mFirstParam;
};


//***********************************************************************************
// 
//

static Plug *plug = NULL;


//***********************************************************************************
// LADSPA methods
//

// This is the ladspa MAIN entry point
VST_EXPORT const LADSPA_Descriptor *ladspa_descriptor( unsigned long index ) {
	//printf("ladspa MAIN: index=%d", index);

	//TODO: this singleton stuff may cause problems when more than one different java plug 
	//is used at the same time --> TEST it!
	if (plug==NULL) {
  		plug = new Plug();
  		
  		AEffect* effect = jvst_main(audioMaster);
  		if( effect && effect->magic == kEffectMagic );
  		else log("** LADSPA: Error during jvst_main");
  		
  		char effectName[256] = {0};
  		effect->dispatcher (effect, effGetEffectName, 0, 0, effectName, 0);
  		
  		bool success = plug->Init(100000 + (rand() % 100000), effectName, effect, effect);
  		
  		if (success==false) {
  			log("** LADSPA: Error wrapping VST Plugin");
  			delete plug;
  			plug = NULL;
  			return NULL;
  		}
	}
  	
 	if (index == 0)
   		return plug->GetDescriptor();
	else
		return NULL; //one plug per .so
}


static LADSPA_Handle vst_instantiate(const LADSPA_Descriptor *desc, unsigned long rate) {
   printf("LADSPA instantiate\n");
   Plug *plug = (Plug *) desc->ImplementationData;
   return plug->Instantiate( rate );
}

static void vst_activate(LADSPA_Handle handle) {
	//show GUI here???
	printf("LADSPA activate\n");	
}

static void vst_connect_port(LADSPA_Handle handle, unsigned long port, LADSPA_Data *data) {
   printf("LADSPA connectPort\n");
   Instance *inst = (Instance *) handle;
   inst->Connect_Port( port, data );
}

static void vst_run(LADSPA_Handle handle, unsigned long count) {
   Instance *inst = (Instance *) handle;
   inst->Run( count );
}

static void vst_run_adding(LADSPA_Handle handle, unsigned long count) {
   Instance *inst = (Instance *) handle;
   inst->Run_Adding( count );
}

static void vst_set_run_adding_gain(LADSPA_Handle handle, LADSPA_Data gain) {
	printf("LADSPA setRunAddingAgain\n");
   	Instance *inst = (Instance *) handle;
   	inst->Set_Run_Adding_Gain( gain );
}

static void vst_deactivate(LADSPA_Handle handle) {
	//hide GUI here???
	printf("LADSPA DEactivate\n");
}

static void vst_cleanup(LADSPA_Handle handle) {
	printf("LADSPA cleanup\n");
   	Instance *inst = (Instance *) handle;
   	inst->Cleanup();
   	delete inst;
}



//***********************************************************************************
// This class models a PLUGIN
//

Plug::Plug() {
   mLib = NULL;
   mPrev = NULL;
   mEffect = NULL;
   mName = NULL;
   memset( &mDesc, 0, sizeof( mDesc ) );
}

Plug::~Plug() {
   unsigned long i;

   if( mDesc.Label ) {
      delete mDesc.Label;
   }

   if( mDesc.Name ) {
      delete mDesc.Name;
   }

   if( mDesc.Maker ) {
      delete mDesc.Maker;
   }

   if( mDesc.Copyright ) {
      delete mDesc.Copyright;
   }

   for( i = 0; i < mDesc.PortCount; i++ ) {
      if( mDesc.PortNames ) {
         if( mDesc.PortNames[ i ] ) {
            free( (void *) mDesc.PortNames[ i ] );
         }
      }
   }

   if( mDesc.PortDescriptors ) {
      delete [] mDesc.PortDescriptors;
   }
 
   if( mDesc.PortNames ) {
      delete [] mDesc.PortNames;
   }
 
   if( mDesc.PortRangeHints ) {
      delete [] mDesc.PortRangeHints;
   }

   if( mName ) {
      free( mName );
   }
}

bool Plug::Init( unsigned long id, char *name, AEffect *thunk, AEffect *effect ) {
   LADSPA_PortDescriptor *portdescriptors;
   LADSPA_PortRangeHint *hints;
   char **portnames;
   char temp[ 500 ];
   int port;
   int i;
   
   mThunk = thunk;
   mEffect = effect;
   mName = strdup( name );
   if( !mName ) {
      return false;
   }

   mDesc.UniqueID = id;

   mDesc.Label = GetStr( effGetEffectName, name );
   mDesc.Name = GetStr( effGetProductString, name );
   mDesc.Maker = GetStr( effGetVendorString, "None" );
   mDesc.Copyright = GetStr( effVendorSpecific, "None" );

   if( !mDesc.Label || !mDesc.Name || !mDesc.Maker || !mDesc.Copyright ) {
      return false;
   }

   mDesc.PortCount = mEffect->numInputs +
                     mEffect->numOutputs +
                     mEffect->numParams;
   mDesc.PortDescriptors = portdescriptors = new LADSPA_PortDescriptor[ mDesc.PortCount ];
   mDesc.PortNames = portnames = new char *[ mDesc.PortCount ];
   mDesc.PortRangeHints = hints = new LADSPA_PortRangeHint[ mDesc.PortCount ];

   if( !mDesc.PortDescriptors || !mDesc.PortNames || !mDesc.PortRangeHints ) {
      return false;
   }

   mDesc.ImplementationData = (void *) this;

   mDesc.instantiate = vst_instantiate;
   mDesc.connect_port = vst_connect_port;
   mDesc.run = vst_run;
   mDesc.run_adding = vst_run_adding;
   mDesc.set_run_adding_gain = vst_set_run_adding_gain;
   mDesc.cleanup = vst_cleanup;
   mDesc.activate = vst_activate;
   mDesc.deactivate = vst_deactivate;

   log( "LADSPA name '%s'\n", mDesc.Name );
   log( "LADSPA maker '%s'\n", mDesc.Maker );
   log( "LADSPA label '%s'\n", mDesc.Label );
   log( "LADSPA crite '%s'\n", mDesc.Copyright );

   port = 0;

   for( i = 0; i < mEffect->numInputs; i++, port++ ) {
      portdescriptors[ port ] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;

      sprintf( temp, "Audio In %d", i );
      portnames[ port ] = strdup( temp );

      if( !portnames[ port ] ) {
         return false;
      }

      hints[ port ].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
   }

   for( i = 0; i < mEffect->numOutputs; i++, port++ ) {
      portdescriptors[ port ] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

      sprintf( temp, "Audio Out %d", i );

      portnames[ port ] = strdup( temp );
      if( !portnames[ port ] ) {
         return false;
      }

      hints[ port ].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
   }

   for( i = 0; i < mEffect->numParams; i++, port++ ) {
      float val;

      portdescriptors[ port ] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;

      memset( temp, 0, sizeof( temp ) );
      sprintf( temp, "_paramname_%d", i );

      mThunk->dispatcher( mEffect,
                          effGetParamName,
                          i,
                          0,
                          temp,
                          0 );
      temp[ sizeof( temp ) - 1 ] = '\0';

      portnames[ port ] = strdup( temp );
      if( !portnames[ port ] ) {
         return false;
      }

      hints[ port ].HintDescriptor = LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      hints[ port ].LowerBound = 0.0f;
      hints[ port ].UpperBound = 1.0f;
      
      val = mThunk->getParameter( mEffect, i );

      if( val < 0.4f ) {
         if( val < 0.1f ) {
            if( val == 0.0f ) {
               hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_0;
            }
            else {
               hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_MINIMUM;
            }
         }
         else {
            hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_LOW;
         }
      }
      else {
         if( val < 0.6f ) {
            hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_MIDDLE;
         }
         else {
            if( val > 0.9f ) {
               if( val == 1.0f ) {
                  hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_1;
               }
               else {
                  hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_MAXIMUM;
               }
            }
            else {
               hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_HIGH;
            }
         }
      }
   }

   return true;
}

Plug *Plug::GetPrev() {
   return mPrev;
}

void Plug::SetPrev( Plug *prev ) {
   mPrev = prev;
}

LADSPA_Descriptor *Plug::GetDescriptor() {
   return &mDesc;
}

char *Plug::GetStr( VstInt32 opcode, char *dflt ) {
   char str[ 256 ];
   int len;
   int i;
   int dot;
   int slash;

   if( mThunk->dispatcher( mEffect, opcode, 0, 0, str, 0 ) )  {
      if( strlen( str ) > 0 ) {
         return strdup( str );
      }
   }

   len = (int) strlen( dflt );
   dot = -1;
   slash = -1;

   for( i = len - 1; i >= 0; i-- ) {
      if( dot < 0 && dflt[ i ] == '.' ) {
         dot = i;
         continue;
      }

      if( dflt[ i ] != '/' && dflt[ i ] != '\\' ) {
         continue;
      }

      slash = i;
      break;
   }

   if( slash > 0 ) {
      strncpy( str, &dflt[ slash + 1 ], len - slash - 1 );
      str[ len - slash - 1 ] = '\0';
   }
   else {
      strcpy( str, dflt );
   }

   if( dot > 0 ) {
      str[ dot - slash - 1 ] = '\0';
   }

   return strdup( str );
}

LADSPA_Handle Plug::Instantiate( unsigned long rate ) {
   Instance *inst = new Instance;

   if( !inst->Init( mName, rate, mEffect, mThunk) ) {
      delete inst;
      inst = NULL;
   }

   return (LADSPA_Handle) inst;
}


//***********************************************************************************
// This class models a plugin instance
//

Instance::Instance() {
}

Instance::~Instance() {
}

bool Instance::Init( char *name, unsigned long rate, AEffect *peffect, AEffect* pthunk) {
   mEffect = peffect;
   mThunk = pthunk;
   
   mBlockSize = 0;
   mFirstParam = mEffect->numInputs + mEffect->numOutputs;
   mNumPorts = mFirstParam + mEffect->numParams;
   mGain = 1.0f;

   mPorts = new LADSPA_Data *[ mNumPorts ];
   if( mPorts == NULL ) {
      return false;
   }
   memset( mPorts, 0, mNumPorts * sizeof(LADSPA_Data *));

   mThunk->dispatcher( mEffect, effOpen, 0, 0, NULL, 0 );
   mThunk->dispatcher( mEffect, effMainsChanged, 0, 0, NULL, 0 );
   mThunk->dispatcher( mEffect, effSetSampleRate, 0, 0, NULL, (float) rate );
   mThunk->dispatcher( mEffect, effSetBlockSize, 0, 64, NULL, 0 );
   mThunk->dispatcher( mEffect, effMainsChanged, 0, 1, NULL, 0 );

   return true;
}

void Instance::Cleanup() {
   mThunk->dispatcher( mEffect, effClose, 0, 0, NULL, 0 );

   delete [] mPorts;
}

void Instance::Connect_Port( unsigned long port, LADSPA_Data *data ) {
   mPorts[ port ] = data;
}

void Instance::Run( unsigned long count ) {
   Set_Parameters();

   if( mBlockSize != count ) {
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 0, NULL, 0);
      mThunk->dispatcher( mEffect, effSetBlockSize, 0, count, NULL, 0);
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 1, NULL, 0);

      mBlockSize = count;
   }

   mThunk->processReplacing( mEffect,
                             &mPorts[ 0 ],
                             &mPorts[ mEffect->numInputs ],
                             count );
}

void Instance::Run_Adding( unsigned long count ) {
   LADSPA_Data *out[ 2 ];
   int index;
   int ich;
   int och;

   Set_Parameters();

   if( mBlockSize != count ) {
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 0, NULL, 0);
      mThunk->dispatcher( mEffect, effSetBlockSize, 0, count, NULL, 0);
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 1, NULL, 0);

      mBlockSize = count;
   }

   out[ 0 ] = new LADSPA_Data[ count ];
   if( !out[ 0 ] ) {      
      return;
   }

   out[ 1 ] = new LADSPA_Data[ count ];
   if( !out[ 0 ] || !out[ 1 ] ) {
      delete [] out[ 0 ];
      return;
   }

   mThunk->processReplacing( mEffect,
                             &mPorts[ 0 ],
                             &out[ 0 ],
                             count );
   
   for( och = mEffect->numInputs, ich = 0; och < mEffect->numInputs + mEffect->numOutputs ; och++, ich++ ) {
      for( index = 0; index < (int) count; index++ ) {
         mPorts[ och ][ index ] += out[ ich ][ index ] * mGain;
      }
   }

   delete [] out[ 0 ];
   delete [] out[ 1 ];
}

void Instance::Set_Run_Adding_Gain( LADSPA_Data gain ) {
   mGain = gain;
}

void Instance::Set_Parameters() {
   int index;

   for( index = mFirstParam; index < mNumPorts; index++ ) {
      if( mPorts[index] ) {
         mThunk->setParameter( mEffect,
                               index - mFirstParam,
                               *mPorts[ index ] );
      }
   }
}

