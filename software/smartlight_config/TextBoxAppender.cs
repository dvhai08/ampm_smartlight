using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net.Appender;
using log4net.Core;
using System.Windows.Forms;

namespace PJ1000TrackerConfiguration
{
    public class TextBoxAppender : IAppender
    {
        TextBox m_textBox;
        string m_name = "TextBoxAppender";

        public TextBoxAppender(TextBox textBox)
        {
            if (textBox != null)
            {
                m_textBox = textBox;
            }
            else
            {
                throw new ArgumentNullException();
            }
        }

        delegate void DoAppendDelegate(LoggingEvent e);

        public void DoAppend(LoggingEvent e)
        {
            if (m_textBox != null)
            {
                if (m_textBox.InvokeRequired)
                {
                    m_textBox.Invoke(new DoAppendDelegate(DoAppend), e);
                }
                else
                {
                    m_textBox.SelectionStart = m_textBox.TextLength;
                    m_textBox.SelectionLength = 0;
                    if (e.ExceptionObject != null)
                    {
                        m_textBox.SelectedText = string.Format("{0} {1,-5} {2} - {3} {4}",
                                e.TimeStamp,
                                e.Level,
                                e.LoggerName,
                                e.MessageObject.ToString(),
                                e.ExceptionObject.ToString() + Environment.NewLine);
                    }
                    else
                    {
                        m_textBox.SelectedText = e.MessageObject.ToString();
                    }
                }
            }
        }

        public void Close() 
        {
            m_textBox = null;
        }

        public string Name
        {
            get { return m_name; }
            set { m_name = value; }
        }
    }
}
