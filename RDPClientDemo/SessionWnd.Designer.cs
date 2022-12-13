namespace WinViewer
{
    partial class SessionWnd 
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SessionWnd));
            this.rdpViewer = new AxRDPCOMAPILib.AxRDPViewer();
            ((System.ComponentModel.ISupportInitialize)(this.rdpViewer)).BeginInit();
            this.SuspendLayout();
            // 
            // rdpViewer
            // 
            this.rdpViewer.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
            this.rdpViewer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rdpViewer.Enabled = true;
            this.rdpViewer.Location = new System.Drawing.Point(12, 12);
            this.rdpViewer.Name = "rdpViewer";
            this.rdpViewer.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("rdpViewer.OcxState")));
            this.rdpViewer.Size = new System.Drawing.Size(772, 466);
            this.rdpViewer.TabIndex = 2;
            this.rdpViewer.OnConnectionEstablished += new System.EventHandler(this.OnConnectionEstablished);
            this.rdpViewer.OnConnectionFailed += new System.EventHandler(this.OnConnectionFailed);
            this.rdpViewer.OnConnectionTerminated += new AxRDPCOMAPILib._IRDPSessionEvents_OnConnectionTerminatedEventHandler(this.OnConnectionTerminated);
            this.rdpViewer.OnError += new AxRDPCOMAPILib._IRDPSessionEvents_OnErrorEventHandler(this.OnError);
            // 
            // SessionWnd
            // 
            this.ClientSize = new System.Drawing.Size(796, 490);
            this.Controls.Add(this.rdpViewer);
            this.Name = "SessionWnd";
            this.Text = "RDP session viewer";
            this.ClientSizeChanged += new System.EventHandler(this.OnClientSizeChanged);
            ((System.ComponentModel.ISupportInitialize)(this.rdpViewer)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private AxRDPCOMAPILib.AxRDPViewer rdpViewer;
    }
}