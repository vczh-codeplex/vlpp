using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using GaclibWebPage.Models;

namespace GaclibWebPage.Controllers
{
    public class HomeController : Controller
    {
        public ActionResult Index(NavigationBarModel model)
        {
            model.ActiveButton = NavigationButtons.Home;
            return View(model);
        }
    }
}
