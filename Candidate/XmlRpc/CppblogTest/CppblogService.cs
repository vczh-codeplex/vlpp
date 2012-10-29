using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using XmlRpc;

namespace CppblogTest
{
    [XmlRpcService]
    public interface ICppblog
    {
        [XmlRpcMethod("blogger.deletePost")]
        bool DeletePost(string appKey, string postid, string username, string password, bool publish);

        [XmlRpcMethod("blogger.getUsersBlogs")]
        BlogInfo[] GetUsersBlogs(string appKey, string username, string password);

        [XmlRpcMethod("metaWeblog.editPost")]
        bool EditPost(string postid, string username, string password, Post post, bool publish);

        [XmlRpcMethod("metaWeblog.getCategories")]
        CategoryInfo[] GetCategories(string blogid, string username, string password);

        [XmlRpcMethod("metaWeblog.getPost")]
        Post GetPost(string postid, string username, string password);

        [XmlRpcMethod("metaWeblog.getRecentPosts")]
        Post[] GetRecentPosts(string blogid, string username, string password, int numberOfPosts);

        [XmlRpcMethod("metaWeblog.newMediaObject")]
        MediaObjectUrl NewMediaObject(string blogid, string username, string password, MediaObject mediaObject);

        [XmlRpcMethod("metaWeblog.newPost")]
        string NewPost(string blogid, string username, string password, Post post, bool publish);
    }

    public class BlogInfo
    {
        public string blogid { get; set; }
        public string url { get; set; }
        public string blogName { get; set; }
    }

    public class Post
    {
        public DateTime dateCreated { get; set; }
        public string description { get; set; }
        public string title { get; set; }
        public string[] categories { get; set; }
        public Enclosure enclosure { get; set; }
        public string link { get; set; }
        public string permalink { get; set; }
        public int postid { get; set; }
        public Source source { get; set; }
        public string userid { get; set; }
    }

    public class CategoryInfo
    {
        public string description { get; set; }
        public string htmlUrl { get; set; }
        public string rssUrl { get; set; }
        public string title { get; set; }
        public string categoryid { get; set; }
    }

    public class MediaObject
    {
        public string name { get; set; }
        public string type { get; set; }
        public string bits { get; set; }
    }

    public class MediaObjectUrl
    {
        public string url { get; set; }
    }

    public class Enclosure
    {
        public int length { get; set; }
        public string type { get; set; }
        public string url { get; set; }
    }

    public class Source
    {
        public string name { get; set; }
        public string url { get; set; }
    }
}
