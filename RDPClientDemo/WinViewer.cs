using System;
using System.IO;
using System.Windows.Forms;

namespace WinViewer
{
    
    public partial class WinViewer : Form
    {
        private SessionWnd sessionWnd;

        public WinViewer()
        {
            InitializeComponent();
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            if (sessionWnd != null)
                return;

            string invitation = ReadFromFile("inv.xml");
            sessionWnd = new SessionWnd(this);

            try
            {
                const string MessagingChannelName = "mes";
                sessionWnd.Connect(invitation, "SessionViewer", MessagingChannelName);
                this.ConnectButton.Enabled = false;
                this.DisconnectButton.Enabled = true;
            }
            catch (Exception ex)
            {
                LogTextBox.Text += ("Error in Connecting. Error Info: " + ex.ToString() + Environment.NewLine);
            }
        }

        public void DisconnectButton_Click(object sender, EventArgs e)
        {
            sessionWnd?.Disconnect();
            this.ConnectButton.Enabled = true;
            this.DisconnectButton.Enabled = false;
        }

        private string ReadFromFile(string fileName)
        {
            string readText = null;

            LogTextBox.Text += ("Reading the connection string from the file name " +
                fileName + Environment.NewLine);
            try
            {
                using (StreamReader sr = File.OpenText(fileName))
                {
                    readText = sr.ReadToEnd();
                }
            }
            catch (Exception ex)
            {
                LogTextBox.Text += ("Error in Reading input file. Error Info: " + ex.ToString() + Environment.NewLine);
            }
            return readText;
        }
        public void InvalidateSessionViewer() { 
            sessionWnd = null;
            this.ConnectButton.Enabled = true;
            this.DisconnectButton.Enabled = false;
        }
    }
}
