import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Applet extends JApplet implements ActionListener {

	protected JTextField textField;
	protected JTextArea textArea;
	protected JComboBox channelBox;
	protected JScrollPane scrollPane;

	public void init() {
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					createGUI();
				}
			});
		} catch (Exception e) {
			System.err.println("CreateGUI" + e.getMessage());
		}
	}

	private void createGUI() {
		Container pane = getContentPane();
		pane.setLayout(new BorderLayout());

		textField = new JTextField(30);
		textField.addActionListener(this);

		textArea = new JTextArea(5, 30);
		textArea.setEditable(false);

		channelBox = new JComboBox();
		channelBox.addItem("Info");
		channelBox.addItem("General");
		channelBox.setSelectedItem("General");

		scrollPane = new JScrollPane(textArea,
			JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

		Panel end = new Panel();
		end.setLayout(new BorderLayout());
		end.add(textField, BorderLayout.CENTER);
		end.add(channelBox, BorderLayout.LINE_END);

		pane.add(scrollPane, BorderLayout.CENTER);
		pane.add(end, BorderLayout.PAGE_END);
	}

	public void actionPerformed(ActionEvent evt) {
		JScrollBar scrollBar = scrollPane.getVerticalScrollBar();
		boolean follow = (scrollBar.getMaximum() == scrollBar.getValue());

		String text = textField.getText();
		textArea.append(text + "\n");
		textField.setText("");

		if (follow) scrollBar.setValue(scrollBar.getMaximum());
	}
}
