using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NodeServiceTest.Endpoints;
using NodeServiceTest.EndpointRequests;
using NodeService;
using NodeService.Endpoints;

namespace NodeServiceTest
{
    [TestClass]
    public class StrongTypedNodeEndpointTest
    {
        [TestMethod]
        public void TestSimpleEndpointSerialization()
        {
            CalculationEndpoint calculation = new CalculationEndpoint(false);
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

        [TestMethod]
        public void TestSimpleEndpointClient()
        {
            CalculationEndpoint calculation = new CalculationEndpoint(false);
            Assert.AreEqual("Calculation", calculation.EndpointName);
            Assert.AreEqual(false, calculation.EnableAsynchronization);

            PrimitiveEndpointClientProvider provider = new PrimitiveEndpointClientProvider(calculation);
            ICalculationEndpoint calculationEndpoint = StrongTypedNodeEndpointClientBuilder.Create<ICalculationEndpoint>(provider);

            Assert.AreEqual(3, calculationEndpoint.Add(2, 1));
            Assert.AreEqual(1, calculationEndpoint.Sub(2, 1));
            Assert.AreEqual(2, calculationEndpoint.Mul(2, 1));
            Assert.AreEqual(2, calculationEndpoint.Div(2, 1));

            calculationEndpoint.SendMessage("Vczh is a genius!");
            Assert.AreEqual("Vczh is a genius!", calculationEndpoint.ReceiveMessage());
            Assert.AreEqual("Vczh is a genius!", calculation.Message);
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidOperationException))]
        public void TestSimpleEndpointClientThrowException()
        {
            CalculationEndpoint calculation = new CalculationEndpoint(false);
            Assert.AreEqual("Calculation", calculation.EndpointName);
            Assert.AreEqual(false, calculation.EnableAsynchronization);

            PrimitiveEndpointClientProvider provider = new PrimitiveEndpointClientProvider(calculation);
            ICalculationEndpoint calculationEndpoint = StrongTypedNodeEndpointClientBuilder.Create<ICalculationEndpoint>(provider);
            calculationEndpoint.ThrowException();
        }
    }
}
