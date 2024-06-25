import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;

@SuppressWarnings("serial")
public class GUIHashRow extends JPanel {
	
	private CengHashRow hashRow;
	
	public GUIHashRow(CengHashRow hashRow)
	{
		this.hashRow = hashRow;
		
		setupStyle();
		setupWithRow();
		
		this.revalidate();
		this.repaint();
	}

	private void setupStyle()
	{		
		if(this.hashRow.isVisited())
		{
			Border border = BorderFactory.createLineBorder(Color.GREEN, 2);

			this.setBorder(border);
		}
		else
		{
			Border border = BorderFactory.createLineBorder(Color.BLACK, 2);
			
			this.setBorder(border);
		}
		
		this.setMinimumSize(new Dimension(200, 60));
		this.setMaximumSize(new Dimension(200, 60));
	}
	
	private void setupWithRow()
	{
		JLabel prefixLabel = new JLabel(this.hashRow.hashPrefix());
		prefixLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
		
		this.add(prefixLabel);
	}
	
	public CengBucket bucketForRow()
	{
		return this.hashRow.getBucket();
	}
	
	public Boolean isVisited()
	{
		return this.hashRow.isVisited();
	}
}
