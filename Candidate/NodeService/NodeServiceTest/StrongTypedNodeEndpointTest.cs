using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NodeServiceTest.Endpoints;
using NodeServiceTest.EndpointRequests;

namespace NodeServiceTest
{
    [TestClass]
    public class StrongTypedNodeEndpointTest
    {
        [TestMethod]
        public void TestSimpleEndpointSerialization()
        {
            CalculationEndpoint calculation = new CalculationEndpoint();
            Assert.AreEqual("Calculation", calculation.EndpointName);
            Assert.AreEqual(false, calculation.EnableAsynchronization);

            calculation.QueueRequest(
                new PrimitiveEndpointRequest<int>(
                    calculation,
                    "Add",
                    (i) => Assert.AreEqual(3, i)
                )
                .AddParameter("a", 2)
                .AddParameter("b", 1)
                );

            calculation.QueueRequest(
                new PrimitiveEndpointRequest<int>(
                    calculation,
                    "Sub",
                    (i) => Assert.AreEqual(1, i)
                )
                .AddParameter("a", 2)
                .AddParameter("b", 1)
                );

            calculation.QueueRequest(
                new PrimitiveEndpointRequest<int>(
                    calculation,
                    "Mul",
                    (i) => Assert.AreEqual(2, i)
                )
                .AddParameter("a", 2)
                .AddParameter("b", 1)
                );

            calculation.QueueRequest(
                new PrimitiveEndpointRequest<int>(
                    calculation,
                    "Div",
                    (i) => Assert.AreEqual(2, i)
                )
                .AddParameter("a", 2)
                .AddParameter("b", 1)
                );
        }
    }
}
