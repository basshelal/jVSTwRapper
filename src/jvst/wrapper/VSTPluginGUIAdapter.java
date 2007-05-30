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

package jvst.wrapper;

import java.awt.Dimension;
import java.awt.Toolkit;
import javax.swing.JFrame;
import jvst.wrapper.gui.VSTPluginGUI;

public abstract class VSTPluginGUIAdapter extends JFrame implements VSTPluginGUI {

	// idea borrowed from apple developer connection
	public static final boolean RUNNING_MAC_X = System.getProperty("os.name").toLowerCase().startsWith("mac os x");
	public static boolean libraryOk = false;

	public boolean WindowAttached;

	public VSTPluginGUIAdapter() {
		// center window
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension frameSize = this.getSize();

		if (frameSize.height > screenSize.height) frameSize.height = screenSize.height;
		if (frameSize.width > screenSize.width) frameSize.width = screenSize.width;
		this.setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);

		// close behavior
		this.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);

		// IMPORTANT: hide window
		this.hide(); // keep hide() to be backward compatible to 1.4!

		// Prepare attaching window
		WindowAttached = false;
		if (!libraryOk) {
			// Load library for native-awt methods
			try {
				System.loadLibrary("jawt");
				libraryOk = true;
			} catch (Exception e) {
				libraryOk = false;
			} catch (Error e) {
				// Maybe library is already loaded
				//TODO: find a better way!
				if (e.getMessage().indexOf("already loaded") != -1) libraryOk = true;
				else libraryOk = false;
			}
		}

		log("\nJAVA GUI Plugin intitialised properly!");
	}

	public void undecorate() {
		if ((this.WindowAttached) && (!this.isUndecorated())) {
			javax.swing.JFrame f = new javax.swing.JFrame();
			f.pack();
			this.dispose();
			this.setUndecorated(true);
			this.pack();
			f.dispose();
			
			java.awt.Rectangle bounds = this.getBounds();
			bounds.x = 0;
			bounds.y = 0;
			this.setBounds(bounds);
		} 
		else if ((!this.WindowAttached) && (this.isUndecorated())) {
			// Redecorate the window
			javax.swing.JFrame f = new javax.swing.JFrame();
			f.pack();
			this.dispose();
			this.setUndecorated(false);
			this.pack();
			f.dispose();
			
			// center window
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			Dimension frameSize = this.getSize();

			if (frameSize.height > frameSize.height) frameSize.height = screenSize.height;
			if (frameSize.width > screenSize.width) frameSize.width = screenSize.width;
			
			java.awt.Rectangle bounds = this.getBounds();
			bounds.x = (screenSize.width - frameSize.width) / 2;
			bounds.y = (screenSize.height - frameSize.height) / 2;
			this.setBounds(bounds);
		}
	}

	// *********************************************************************
	public void open() {
		log("GUI open");
		this.show(); // keep show() to be backward compatible to 1.4!
		this.toFront();
	}

	public void close() {
		this.log("GUI close");
		this.hide(); // keep hide() to be backward compatible to 1.4!
	}

	public void destroy() {
		this.log("GUI destroy");

		// NOTE: This method is currently not called in the jvstwrapper
		// macx implementation! It caused some hosts to block forever...
		// TODO: check again now, the gui integration stuff has changed
		// significantly on the mac
		if (!RUNNING_MAC_X) {
			this.dispose();
		}
	}

	// ***********************************************************************
	protected void log(String s) {
		VSTPluginAdapter.log(s);
	}

}
