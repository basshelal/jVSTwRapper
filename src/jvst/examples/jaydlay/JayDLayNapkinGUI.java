/* 
 * jVSTwRapper - The Java way into VST world!
 * 
 * jVSTwRapper is an easy and reliable Java Wrapper for the Steinberg VST interface. 
 * It enables you to develop VST 2.3 compatible audio plugins and virtual instruments 
 * plus user interfaces with the Java Programming Language. 3 Demo Plugins(+src) are included!
 * 
 * Copyright (C) 2006  Daniel Martin [daniel309@users.sourceforge.net] 
 * 					   and many others, see CREDITS.txt
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

package jvst.examples.jaydlay;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import net.sourceforge.napkinlaf.NapkinLookAndFeel;

public class JayDLayNapkinGUI extends JayDLayGUI {

	private static final long serialVersionUID = -1315625186980889087L;




	public JayDLayNapkinGUI() throws Exception {
		//throw exception that the user gets a message box with the 
		//stack trace if something fails in the constructor here...
		
		this.setSize(230, 200);
		
		//TELL the UI Manager that it should use the Plugins ClassLoader to load UI classes. 
		UIManager.put("ClassLoader", this.getClass().getClassLoader());

		//use the napkin look-and-feel for this plugin!
		UIManager.setLookAndFeel(new NapkinLookAndFeel());
		SwingUtilities.updateComponentTreeUI(this);
	}


	
	
	public static void main(String[] args) throws Throwable {
		DEBUG=true;
		
		JayDLayNapkinGUI gui = new JayDLayNapkinGUI();
	    gui.init(null);
	    gui.open();
	}

}
