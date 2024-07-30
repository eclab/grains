// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


import javax.sound.midi.*;
import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.io.*;
import javax.sound.midi.*;
import javax.sound.midi.spi.*;


/** This class is just a utility to convert .mid files to .txt header files sufficient to 
	cut and paste for ARIA.  The .mid files must be MIDI type 1 I believe.  
	Notes can be  be sixteenth notes (or rests)
	starting at timestep 0.  You cannot have more than 256 sixteenth notes per key.
*/

public class Aria
	{
	JComponent parent;
	
	public Aria(JComponent parent) { parent = this.parent; }	
	
	//// HANDLING ERROR MESSAGES
	
    ArrayList<JMenuItem> disabledMenus = null;
    int disableCount;
    /** Disables the menu bar.  disableMenuBar() and enableMenuBar() work in tandem to work around
        a goofy bug in OS X: you can't disable the menu bar and reenable it: it won't reenable
        unless the application loses focus and regains it, and even then sometimes it won't work.
        These functions work properly however.  You want to disable and enable the menu bar because
        in OS X the menu bar still functions even when in a modal dialog!  Bad OS X Java errors.
    */
    public void disableMenuBar()
        {
        if (disabledMenus == null)
            {
            disabledMenus = new ArrayList<JMenuItem>();
            disableCount = 0;
            JFrame ancestor = ((JFrame)(SwingUtilities.getWindowAncestor(parent)));
            if (ancestor == null) return;
            JMenuBar bar = ancestor.getJMenuBar();
            for(int i = 0; i < bar.getMenuCount(); i++)
                {
                JMenu menu = bar.getMenu(i);
                if (menu != null)
                    {
                    for(int j = 0; j < menu.getItemCount(); j++)
                        {
                        JMenuItem item = menu.getItem(j);
                        if (item != null && item.isEnabled())
                            {
                            disabledMenus.add(item);
                            item.setEnabled(false);
                            }
                        }
                    }
                }
            }
        else
            {
            disableCount++;
            return;
            }
        }       
        
    /** Enables the menu bar.  disableMenuBar() and enableMenuBar() work in tandem to work around
        a goofy bug in OS X: you can't disable the menu bar and reenable it: it won't reenable
        unless the application loses focus and regains it, and even then sometimes it won't work.
        These functions work properly however.  You want to disable and enable the menu bar because
        in OS X the menu bar still functions even when in a modal dialog!  Bad OS X Java errors.
    */
    public void enableMenuBar()
        {
        if (disableCount == 0)
            {
            for(int i = 0; i < disabledMenus.size(); i++)
                {
                disabledMenus.get(i).setEnabled(true);
                }
            disabledMenus = null;
            }
        else
            {
            disableCount--;
            }
        }       

	boolean inSimpleError = false;
    /** Display a simple error message. */
    public void showSimpleError(String title, String message)
        {
        // A Bug in OS X (perhaps others?) Java causes multiple copies of the same Menu event to be issued
        // if we're popping up a dialog box in response, and if the Menu event is caused by command-key which includes
        // a modifier such as shift.  To get around it, we're just blocking multiple recursive message dialogs here.
        
        if (inSimpleError) return;
        inSimpleError = true;
        disableMenuBar();
        JOptionPane.showMessageDialog(parent, message, title, JOptionPane.ERROR_MESSAGE);
        enableMenuBar();
        inSimpleError = false;
        }

    /** Display a simple error message. */
    public void showSimpleMessage(String title, String message)
        {
        // A Bug in OS X (perhaps others?) Java causes multiple copies of the same Menu event to be issued
        // if we're popping up a dialog box in response, and if the Menu event is caused by command-key which includes
        // a modifier such as shift.  To get around it, we're just blocking multiple recursive message dialogs here.
        
        if (inSimpleError) return;
        inSimpleError = true;
        disableMenuBar();
        JOptionPane.showMessageDialog(parent, message, title, JOptionPane.INFORMATION_MESSAGE);
        enableMenuBar();
        inSimpleError = false;
        }
        
        
        
    //// IDENTIFYING NOTES FROM MIDI MESSAGES

    /** Returns true if the given message is a non-null ShortMessage of the type NOTE_ON with a non-zero velocity. */
    public boolean isNoteOn(MidiMessage message)
        {
        if (message == null) return false;
        if (!(message instanceof ShortMessage)) return false;
        ShortMessage shortMessage = (ShortMessage) message;
        return (shortMessage.getCommand() == ShortMessage.NOTE_ON && shortMessage.getData2() > 0);
        }


    /** Returns true if the given message is a non-null ShortMessage of the type NOTE_OFF or
        is a NOTE_ON with a zero velocity. */
    public boolean isNoteOff(MidiMessage message)
        {
        if (message == null) return false;
        if (!(message instanceof ShortMessage)) return false;
        ShortMessage shortMessage = (ShortMessage) message;
        return ((shortMessage.getCommand() == ShortMessage.NOTE_ON && shortMessage.getData2() == 0) ||
            (shortMessage.getCommand() == ShortMessage.NOTE_OFF));
        }


	public static String[] KEYS = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };
	public class Note
		{
		public int pos = -1;
		public int len = -1;
		public int pitch = -1;
		public Note(int pos, int pitch) { this.pos = pos; this.pitch = pitch; }
		public String pitch() 
			{ 
			return "_" + KEYS[(pitch - 60) % 12] + (pitch - 60 < 12 ? "" : (pitch - 60) / 12 + 1) + " "; 
			}

		public String toString()
			{
			if (len == 1) return pitch();
			if (len == 2) return "_N8, " + pitch();
			if (len == 4) return "_N4, " + pitch();
			if (len == 8) return "_N2, " + pitch();
			if (len == 16) return "_N1, " + pitch();
			
			// Now it's uglier
			boolean didPitch = false;
			String str = "";
			for(int i = len; i > 0; )
				{
				if (i >= 16)
					{
					str += "_N1, ";
					if (didPitch) str += "TIE, ";
					else { str += pitch(); didPitch = true; }
					i -= 16;
					}
				else if (i >= 8)
					{
					str += "_N2, ";
					if (didPitch) str += "TIE, ";
					else { str += pitch(); didPitch = true; }
					i -= 8;
					}
				else if (i >= 4)
					{
					str += "_N4, ";
					if (didPitch) str += "TIE, ";
					else { str += pitch(); didPitch = true; }
					i -= 4;
					}
				else if (i >= 2)
					{
					str += "_N8, ";
					if (didPitch) str += "TIE, ";
					else { str += pitch(); didPitch = true; }
					i -= 2;
					}
				else if (i >= 1)
					{
					if (didPitch) str += "TIE, ";
					else { str += pitch(); didPitch = true; }
					i -= 1;
					}
				}
			return str;
			}
		}
		
	public String rest(int len)
		{
		if (len == 1) return "_R,";
		if (len == 2) return "_R8, ";
		if (len == 4) return "_R4, ";
		if (len == 8) return "_R2, ";
		if (len == 16) return "_R1, ";
	
		// Now it's uglier
		boolean didRest = false;
		String str = "";
		for(int i = len; i > 0; )
			{
			if (i >= 16)
				{
				if (didRest) str += "_N8, TIE, ";
				else { str += "_R8, "; didRest = true; }
				i -= 16;
				}
			else if (i >= 8)
				{
				if (didRest) str += "_N4, TIE, ";
				else { str += "_R4, "; didRest = true; }
				i -= 8;
				}
			else if (i >= 4)
				{
				if (didRest) str += "_N2, TIE, ";
				else { str += "_R2, "; didRest = true; }
				i -= 4;
				}
			else if (i >= 2)
				{
				if (didRest) str += "_N1, TIE, ";
				else { str += "_R1, "; didRest = true; }
				i -= 2;
				}
			else if (i >= 1)
				{
				if (didRest) str += "TIE, ";
				else { str += "_R, "; didRest = true; }
				i -= 1;
				}
			}
		return str;
		}
		
	/// READING THE MIDI FILE AND LOADING THE ARRAY

	public boolean read(Sequence sequence)
		{
		boolean warnedNotes = false;
		Track[] tracks = sequence.getTracks();
		if (tracks.length == 0)
			{
			showSimpleError("No Data", "This MIDI file has no data.");
			return false;
			}
		if (sequence.getDivisionType() != Sequence.PPQ) 
			{
			showSimpleError("Incorrect Timing", "This MIDI file uses timing other than Parts Per Quarter Note (PPQ).  Probably SMPTE.");
			return false;
			}
		if (tracks.length > 1) 
			{
			showSimpleMessage("Multiple Tracks", "This MIDI file has " + tracks.length + " tracks.  Only the first one will be read.");
			}
		int resolution = sequence.getResolution();
		
		Note lastNote = null;
		ArrayList<Note> notes = new ArrayList<>();
				
        // FIXME: At present we only load a single track
        for(int i = 0; i < tracks[0].size(); i++)
            {
            MidiEvent e = tracks[0].get(i);
            int pos = (int)(e.getTick() / (resolution / 4));	// This should be number of sixteenth notes
            MidiMessage message = e.getMessage();
            if (message instanceof ShortMessage)
                {
                ShortMessage shortMessage = (ShortMessage)message;
				if (isNoteOn(shortMessage))
					{
                    int pitch = shortMessage.getData1();
                    
                    if (pitch < 60)
                    	{
                    	System.err.println("NOTE WITH PITCH LESS THAN MIDDLE C: " + pitch);
                    	}
                    else if (pitch > 41)
                    	{
                    	System.err.println("NOTE WITH PITCH HIGHER THAN F 3 OCTAVES ABOVE MIDDLE C: " + pitch);
                    	}
                    else if (lastNote== null)
                    	{
                    	lastNote = new Note(pos, pitch);
						System.err.println("" + pitch + " " + pos);
                    	}
                    else
                    	{
                    	System.err.println("OVERLAPPING NOTE " + pitch);
                    	}
					}
					
				if (isNoteOff(shortMessage))
					{
                    int pitch = shortMessage.getData1();
                    
                    if (lastNote == null)
                    	{
                    	System.err.println("NO NOTE " + pitch);
                    	}
                    else if (lastNote.pitch != pitch)
                    	{
                    	System.err.println("NO NOTE FOR PITCH " + pitch);
                    	lastNote = null;		// otherwise we might fail for the rest of the sequence
                    	}
                    else
                    	{
                    	lastNote.len = pos - lastNote.pos;
                    	if (lastNote.len <= 0)
                    		{
                    		System.err.println("LENGTH TOO SHORT FOR PITCH " + pitch);
                    		}
                    	else
                    		{
                    		notes.add(lastNote);
                    		}
                    	lastNote = null;
                    	}
					}
					
				}
            }
        
        StringBuilder build = new StringBuilder();
        build.append("{\t");
        
		// Now we need to write out notes and maybe rests
		int count = 0;
		final int MAX_COUNT = 8;
		for(int i = 0; i < notes.size(); i++)
			{
			Note note = notes.get(i);
			if (note.len == 1)
				{
				if (i > 1)
					{
					// Do we have a rest?
					int space = note.pos - (lastNote.pos + lastNote.len);
					if (space > 0)
						{
						build.append(rest(space));
						}
					count++;
					if (count > MAX_COUNT)
						{
						build.append("\n\t");
						count = 0;
						}
					}
				build.append(note.toString());
				count++;
				if (count > MAX_COUNT)
					{
					build.append("\n\t");
					count = 0;
					}
				lastNote = note;
				}
			}
		build.append(" },\n");
        System.err.println(build);
        return true;
        }
		
		
		
	//// WRITING THE C HEADER FILE
		
	public boolean write(File file)
		{
		return true;
		}
		
		
	//// TOP LEVEL
	//// doit() is called by a menu option.  It presents an OPEN FILE dialog box, then loads the MIDI file,
	//// then presents a SAVE FILE dialog box, then saves the .txt file.
		
	
    /** Returns a string which guarantees that the given filename ends with the given ending. */   
    public static String ensureFileEndsWith(String filename, String ending)
        {
        // do we end with the string?
        if (filename.regionMatches(false,filename.length()-ending.length(),ending,0,ending.length()))
            return filename;
        else return filename + ending;
        }

    public JFrame getFrame(JComponent parent) { return (JFrame) SwingUtilities.getWindowAncestor(parent); }

	public void doit()
		{
		// Load File
		FileDialog fd = new FileDialog(getFrame(parent), "Load MIDI File...", FileDialog.LOAD);
		fd.setFilenameFilter(new FilenameFilter()
			{
			public boolean accept(File dir, String name)
				{
				return ensureFileEndsWith(name, ".mid").equals(name);
				}
			});

		disableMenuBar();
		fd.setVisible(true);
		enableMenuBar();
		
		if (fd.getFile() != null)
			{
			Sequence sequence = null;
			try
				{
	    		sequence = MidiSystem.getSequence(fd.getFiles()[0]);
	    		}
	    	catch (IOException ex)
	    		{
	    		ex.printStackTrace();
				showSimpleError("Error Reading from File", "Could not read the file.");
				return;
	    		}
	    	catch (InvalidMidiDataException ex)
	    		{
	    		ex.printStackTrace();
				showSimpleError("Invalid MIDI", "The file does not contain valid MIDI.");
				return;
	    		}
    		if (!read(sequence)) return;
    		}
    	
        FileDialog fd2 = new FileDialog(getFrame(parent), "Save Aria Data...", FileDialog.SAVE);
                
        disableMenuBar();
        fd2.setVisible(true);
        enableMenuBar();
                
        if (fd2.getFile() != null)
            {
            try
            	{
				File f = new File(fd2.getDirectory(), ensureFileEndsWith(fd2.getFile(), ".txt"));
				write(f);
				}
			catch (Exception ex)
				{
				ex.printStackTrace();
				showSimpleError("Error Writing to File", "Could not create the file.");
				}
			finally
				{
				// do nothing
				}
			}
		}
	}