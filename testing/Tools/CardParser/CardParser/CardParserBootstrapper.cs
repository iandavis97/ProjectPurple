using System.Windows;
using Caliburn.Micro;

namespace CardParser
{
    // http://caliburnmicro.com/documentation

    public class CardParserBootstrapper : BootstrapperBase
    {
        public CardParserBootstrapper()
        {
            Initialize();
        }

        protected override void OnStartup(object sender, StartupEventArgs e)
        {
            DisplayRootViewFor<CardParserViewModel>();
        }
    }
}