using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using log4net.Core;

namespace PJ1000TrackerConfiguration
{
    public static class Logger
    {
		private static TextBoxAppender logAppender = null;
        public static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		public static void addTextBoxAppender(TextBox textBox)
		{			
            log4net.Repository.Hierarchy.Hierarchy repository;

            if (logAppender == null)
            {
                logAppender = new TextBoxAppender(textBox);
                repository = (log4net.Repository.Hierarchy.Hierarchy)log4net.LogManager.GetRepository();
                repository.Root.AddAppender(logAppender);
                repository.Root.Level = Level.All;
                repository.Configured = true;
                repository.RaiseConfigurationChanged(EventArgs.Empty);
            }
		}

		public static void removeTextBoxAppender()
		{
			if (logAppender != null)
			{
                logAppender.Close();
				((log4net.Repository.Hierarchy.Hierarchy)log4net.LogManager.GetRepository()).Root.RemoveAppender(logAppender);
				logAppender = null;
			}
		}
    }
}
