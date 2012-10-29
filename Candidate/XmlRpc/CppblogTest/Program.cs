using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using XmlRpc;
using System.Xml.Linq;
using System.Net;
using System.IO;

namespace CppblogTest
{
    static class Program
    {
        static void Main(string[] args)
        {
            Console.Write("username:");
            string username = Console.ReadLine();
            Console.Write("password:");
            string password = Console.ReadLine();

            Console.WriteLine("Get all post ids...");
            var postIds = GetBlogPostIds(username);
            Console.WriteLine("Get all posts...");
            var posts = GetPosts(username, password, postIds).ToArray();
            Console.WriteLine("Save posts...");
            SavePosts(posts);
        }

        static string[] GetBlogPostIds(string username)
        {
            string url = string.Format("http://www.cppblog.com/{0}/GetBlogPostIds.aspx", username);

            WebRequest req = WebRequest.Create(url);
            req.Method = "GET";

            var resp = req.GetResponse();
            var sr = new StreamReader(resp.GetResponseStream());
            string content = sr.ReadToEnd();
            sr.Close();
            resp.Close();

            return content.Split(',');
        }

        static IEnumerable<Post> GetPosts(string username, string password, string[] postIds)
        {
            string url = string.Format("http://www.cppblog.com/{0}/services/metaweblog.aspx", username);
            ICppblog cppblog = XmlRpcClient.Create<ICppblog>(url);

            int count = 0;
            foreach (var id in postIds)
            {
                Post post = null;
                try
                {
                    post = cppblog.GetPost(id, username, password);
                    Console.WriteLine("{0}/{1}", ++count, postIds.Length);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Post {0}: {1}", id, ex.Message);
                }
                if (post != null)
                {
                    yield return post;
                }
            }
        }

        static void SavePosts(Post[] posts)
        {
            new XDocument(
                new XElement("cppblog",
                    posts.Select(
                        p =>
                            new XElement("post",
                                new XAttribute("date", p.dateCreated),
                                new XAttribute("title", p.title),
                                new XAttribute("url", p.link),
                                new XElement("categories",
                                    (p.categories ?? new string[0]).Select(c => new XElement("category", c)).ToArray()
                                    ),
                                new XElement("content", p.description)
                                )
                            )
                            .ToArray()
                        )
                    )
                .Save("cppblog.xml");
        }
    }
}
