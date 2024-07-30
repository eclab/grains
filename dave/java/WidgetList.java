// Copyright 2008 by Sean Luke and George Mason University
// Licensed under the Apache 2.0 License


import java.awt.*;
import javax.swing.*;
 
public class WidgetList extends JPanel
    {
    private static final long serialVersionUID = 1;
        
    public WidgetList() { }
                
    public WidgetList(String[] labels, JComponent[] widgets)
        {
        build(labels, widgets);
        }
        
    public void build(String[] labels, JComponent[] widgets)
        {
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        
        for(int i = 0; i < labels.length; i++)
            {
            c.gridx = 0;
            c.gridy = i;
            c.gridwidth = 1;
            c.gridheight = 1;
            c.fill = GridBagConstraints.HORIZONTAL;
            c.anchor = GridBagConstraints.LINE_END;
            c.weightx = 0;
            c.weighty = 1;
            c.insets = new Insets(2, 2, 2, 2);
            panel.add(new JLabel(labels[i] + " ", SwingConstants.RIGHT), c);
                
            c.gridx = 1;
            c.anchor = GridBagConstraints.LINE_START;
            c.weightx = 1;
            panel.add(widgets[i], c);
            }
        
        setLayout(new BorderLayout());
        add(panel, BorderLayout.SOUTH);
        }

    }
        
