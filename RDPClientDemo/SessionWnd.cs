using System;
using System.Windows.Forms;
using AxRDPCOMAPILib;
using RDPCOMAPILib;

namespace WinViewer
{
    public partial class SessionWnd : Form
    {
        private WinViewer controller;
        private IRDPSRAPIVirtualChannel virtualChannel;
        private CTRL_LEVEL currentControlLevel = CTRL_LEVEL.CTRL_LEVEL_VIEW;
        private System.Drawing.Size streamingSize;

        public SessionWnd(WinViewer controller)
        {
            InitializeComponent();
            this.controller = controller;
            this.rdpViewer.SmartSizing = true; //impossible to set from UI editor
        }

        private void OnSharedDesktopSettingsChanged(object sender, _IRDPSessionEvents_OnSharedDesktopSettingsChangedEvent e)
        {
            streamingSize = new System.Drawing.Size(e.width, e.height);
            controller.LogTextBox.Text += $"Streaming rect {{width,height}}: {{{streamingSize.Width},{streamingSize.Height}}}" + Environment.NewLine;

            UpdateRdpViewerSize();
        }

        private void UpdateRdpViewerSize()
        {
            var streamingRatio = Convert.ToDouble(streamingSize.Width) / Convert.ToDouble(streamingSize.Height);
            var rdpMaxWidth = ClientSize.Width - 24;
            var rdpMaxHeight = ClientSize.Height - 24;

            rdpViewer.Width = Math.Min(Convert.ToInt32(rdpMaxHeight * streamingRatio), rdpMaxWidth);
            rdpViewer.Height = Math.Min(Convert.ToInt32(rdpMaxWidth / streamingRatio), rdpMaxHeight);

            rdpViewer.Top = (ClientSize.Height - rdpViewer.Height) / 2;
            rdpViewer.Left = (ClientSize.Width - rdpViewer.Width) / 2;
        }

        private void OnChannelDataReceived(object sender, _IRDPSessionEvents_OnChannelDataReceivedEvent e)
        {
            if (e.bstrData.Contains("Error"))
            {
                var split = e.bstrData.Split(' ');
                if (split.Length > 1)
                {
                    controller.LogTextBox.Text += "Server send an error: " + split[1] + Environment.NewLine;
                }
            }
        }

        public void RequestControl(CTRL_LEVEL level)
        {
            rdpViewer.RequestControl(level);
            currentControlLevel = level;
        }

        public void Disconnect()
        {
            this.Hide();
            rdpViewer.Disconnect();
            this.virtualChannel = null;
            controller.InvalidateSessionViewer();
        }

        public void Connect(string connectionString, string viewerName, string vChannelName)
        {
            if (string.IsNullOrEmpty(connectionString))
                return;
            try
            {
                rdpViewer.Connect(connectionString, viewerName, "");
                this.virtualChannel = rdpViewer.VirtualChannelManager.CreateVirtualChannel(vChannelName, CHANNEL_PRIORITY.CHANNEL_PRIORITY_HI, 0);
            }
            catch (Exception ex)
            {
                controller.LogTextBox.Text += ("Error in Connecting. Error Info: " + ex.ToString() + Environment.NewLine);
            }

        }
        private void OnConnectionEstablished(object sender, EventArgs e)
        {
            this.Show();
            this.rdpViewer.Show();
            controller.LogTextBox.Text += "Connection Established" + Environment.NewLine;
            controller.rootWndConnState.Text = "CTRL_LEVEL_VIEW";

            rdpViewer.OnChannelDataReceived += OnChannelDataReceived;
            rdpViewer.OnSharedDesktopSettingsChanged += OnSharedDesktopSettingsChanged;
        }

        private void OnError(object sender, _IRDPSessionEvents_OnErrorEvent e)
        {
            this.Hide();
            int ErrorCode = (int)e.errorInfo;
            controller.LogTextBox.Text += ("Error 0x" + ErrorCode.ToString("X") + Environment.NewLine);
            controller.rootWndConnState.Text = "DISCONNECTED";
            controller.InvalidateSessionViewer();
        }

        private void OnConnectionTerminated(object sender, _IRDPSessionEvents_OnConnectionTerminatedEvent e)
        {
            rdpViewer.OnChannelDataReceived -= OnChannelDataReceived;
            rdpViewer.OnSharedDesktopSettingsChanged -= OnSharedDesktopSettingsChanged;

            this.Hide();
            controller.LogTextBox.Text += "Connection Terminated. Reason: " + e.discReason + Environment.NewLine;
            controller.rootWndConnState.Text = "DISCONNECTED";
            controller.InvalidateSessionViewer();
        }

        private void OnConnectionFailed(object sender, EventArgs e)
        {
            this.Hide();
            controller.LogTextBox.Text += "Connection Failed." + Environment.NewLine;
            controller.rootWndConnState.Text = "DISCONNECTED";
            controller.InvalidateSessionViewer();
        }

        private void SessionWnd_FormClosing(object sender, System.Windows.Forms.FormClosingEventArgs e)
        {
            this.controller.DisconnectButton_Click(this, new System.EventArgs());
        }

        private void OnClientSizeChanged(object sender, EventArgs e)
        {
            UpdateRdpViewerSize();
        }
    }
}
