import com.fazecast.jSerialComm.*;
import javax.sound.midi.*;
import java.util.*;

public class Dave implements Receiver
	{
	SerialPort port = null;
	
	public void close() { }
	public void send(MidiMessage message, long timestamp)
		{
		if (message instanceof ShortMessage)
			{
			ShortMessage sm = (ShortMessage)message;
			byte[] data = sm.getMessage();
			port.writeBytes(data, data.length);
			}
		}
	
	public static void main(String[] args) throws Exception
		{
		new Dave(args);
		}
		
	public Dave(String[] args) throws Exception
		{
		setupMIDI(args);
		setupSerial(args);
		// MIDI runs on a separate thread so we should be done
		while(true)
			{
			Thread.currentThread().sleep(1000);
			}
		}
	
	public void setupSerial(String[] args) throws Exception
		{
		if (args.length < 2) 
			{
			showMIDIDevices();
			showSerialDevices();
			System.exit(0);
			}
		else
			{
			SerialPort[] ports = SerialPort.getCommPorts();
			port = ports[getInt(args[1])];
			port.setBaudRate(115200);
			port.openPort();
			}
		}
		
    public void setupMIDI(String[] args) throws Exception
        {
		if (args.length == 0) 
			{
			showMIDIDevices();
			showSerialDevices();
			System.exit(0);
			}
		else
			{
			MidiDevice device = getMIDIDevices().get(getInt(args[0]));
			Transmitter transmitter = device.getTransmitter();
			transmitter.setReceiver(this);
			device.open();
			}
        }

    // Parses an integer from a String.  Used by the methods above.
    static int getInt(String s) 
        {
        try {
            return (Integer.parseInt(s));
            } catch (NumberFormatException ex) 
            {
            return -1;
            }
        }

    static ArrayList<MidiDevice> getMIDIDevices() throws Exception
        {
        MidiDevice.Info[] midiDevices = MidiSystem.getMidiDeviceInfo();
        
        ArrayList<MidiDevice> allDevices = new ArrayList<>();
        for(int i = 0; i < midiDevices.length; i++)
            {
			MidiDevice d = MidiSystem.getMidiDevice(midiDevices[i]);
			// get rid of java devices
			if (d instanceof javax.sound.midi.Sequencer ||
				d instanceof javax.sound.midi.Synthesizer)
				continue;
			if (d.getMaxTransmitters() != 0)
				{
				allDevices.add(d);
				}
            }
        return allDevices;
		}


    // Displays all current available input and output MIDI and Audio devices
    static void showMIDIDevices() throws Exception
        {
        MidiDevice.Info[] midiDevices = MidiSystem.getMidiDeviceInfo();
        
        ArrayList<MidiDevice.Info> allDevices = new ArrayList<>();
        for(int i = 0; i < midiDevices.length; i++)
            {
			MidiDevice d = MidiSystem.getMidiDevice(midiDevices[i]);
			// get rid of java devices
			if (d instanceof javax.sound.midi.Sequencer ||
				d instanceof javax.sound.midi.Synthesizer)
				continue;
			if (d.getMaxTransmitters() != 0)
				{
				allDevices.add(midiDevices[i]);
				}
            }

        System.err.println("\nMIDI DEVICES:");
        for(int i = 0; i < allDevices.size(); i++)
        	System.err.println("" + i + "\t" + allDevices.get(i));
        }

    static void showSerialDevices()
        {
        SerialPort[] devices = SerialPort.getCommPorts();
        System.err.println("\nSERIAL DEVICES:");
        for(int i = 0; i < devices.length; i++)
        	System.err.println("" + i + "\t" + devices[i]);
        }

	}