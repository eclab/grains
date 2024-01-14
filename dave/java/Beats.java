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


/** This class is just a utility to convert .mid files to .h header files sufficient to 
	cut and paste for BEATS.  The .mid files must be MIDI type 1 I believe, and they should
	consist solely of the keys for Middle C, D, E, and F.  Notes should be sixteenth notes (or rests)
	starting at timestep 0.  You cannot have more than 256 sixteenth notes per key.
*/

public class Beats
	{
	JComponent parent;
	int max;
	public static final int MAX_LENGTH = 256;
	public static final int NUM_TRACKS = 4;
	int[][] beats = new int[NUM_TRACKS][MAX_LENGTH];

	
	public Beats(JComponent parent) { parent = this.parent; }	
	
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
                    int vel = shortMessage.getData2();
                    
                    if (pitch != 60 && pitch != 62 && pitch != 64 && pitch != 65)
                    	{
                    	System.err.println("Invalid Pitch " + pitch);
                    	if (!warnedNotes)
                    		{
							showSimpleMessage("Invalid Note", "This MIDI file has notes that are not middle C, D, E, or F, which will be skipped.");
                    		warnedNotes = true;
                    		}
                    	continue;
                    	}
                    else if (pos >= MAX_LENGTH)
                    	{
						showSimpleMessage("Incorrectly Timed Data", "This MIDI file has notes that are beyond 256 sixteenth notes.");
                    	break;
                    	}

					if (pitch == 60) beats[0][pos] = (1);
					else if (pitch == 62) beats[1][pos] = (1);
					else if (pitch == 64) beats[2][pos] = (1);
					else if (pitch == 65) beats[3][pos] = (1);
					System.err.println("" + pitch + " " + pos);
					}
				}
            }
            
        // Determine max beat
        for(int i = 0; i < NUM_TRACKS; i++)
        	{
        	for(int j = beats[i].length - 1; j >= 0; j--)
        		{
        		if (beats[i][j] == 1)
        			{
        			if (max < j) max = j;
        			break;
        			}
        		}
        	}
        
        if (max == 0) // uh oh
        	{
			showSimpleError("No Data", "This MIDI file has no data.");
			return false;
        	}
        return true;
		}
		
		
		
	//// WRITING THE C HEADER FILE
		
	public boolean write(File file)
		{
		// convert to length as used by Beats
		if (max == MAX_LENGTH - 1)
			max = 0;
		else max = max + 1; 
		
		try
			{
			PrintWriter writer = new PrintWriter(new FileOutputStream(file));
			writer.println("PROGMEM const struct pattern seq[PATTERNS] =");
			writer.println("\t{");
			writer.println("\t\t{");
			writer.println("\t\t" + max + ",");
			writer.println("\t\t0\t\t///Iterations.  This is >= 1, or FOREVER (0) to loop forever");
			for(int i = 0; i < NUM_TRACKS; i++)
				{
				writer.print("\t\t{ ");
				for(int j = 0; j < max; j++)
					writer.print("" + beats[i][j] + ", ");
				writer.println("},");
				}
			writer.println("\t};");
			writer.close();
			return true;
			}
		catch (IOException ex)
			{
	    	ex.printStackTrace();
			showSimpleMessage("Error Writing to File", "Could not write to the file.");
			return false;
			}
		}
		
		
	//// TOP LEVEL
	//// doit() is called by a menu option.  It presents an OPEN FILE dialog box, then loads the MIDI file,
	//// then presents a SAVE FILE dialog box, then saves the .h file.
		
	
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
    	
        FileDialog fd2 = new FileDialog(getFrame(parent), "Save Beats Data...", FileDialog.SAVE);
                
        disableMenuBar();
        fd2.setVisible(true);
        enableMenuBar();
                
        if (fd2.getFile() != null)
            {
            try
            	{
				File f = new File(fd2.getDirectory(), ensureFileEndsWith(fd2.getFile(), ".h"));
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