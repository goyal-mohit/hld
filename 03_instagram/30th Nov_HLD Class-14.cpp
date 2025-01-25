Date : 30th Nov 2024
Mentor: DEVANG SHARMA
Batch: Sept Batch (DSA) - Tutort Academy

Agenda: System Design - Case Studies

(1) System Design Interviews: A step by step guide: DONE
(2) Designing a URL Shortening service like TinyURL: DONE
(3) Designing Pastebin: DONE

(4) Designing Instagram: DONE
(5) Designing Facebook Feed: DONE
(6) Designing Twitter: DONE

(7) Designing Dropbox/Google Drive/OneDrive
(8) Designing Whatsapp/Messenger/Telegram 

(9) Designing Yelp/Justdial/Sulekha
(10) Designing Uber/Grab/Ola

(11) Design Netflix/Youtube/Hotstar


"Please Type 'Hi' in the Chat Box if you have joined and Can See this Screen".





--------> Case Study-5: Designing Twitter (X)


Twitter:
https://github.com/twitter/the-algorithm/tree/main




Twitter:

- Social Media Platform where users post and read short posts (140 chars) called "Tweets"
- Tweet can support: Pics, Videos, GIFs, Location etc
- Non-Registered Users: Can just read the tweets (Private Tweet vs Public Tweets)
- Registered Users: Read and Write (Like, Comment, Retweet, Share)
- Multi-Platform: Web, Mobile, Smart Watches etc



(1) Demo:

https://x.com
https://twitter.com








(2) Requirement Clarification:


(A) Functional Requirements:

- User should be able to post new tweets
- User should be able to follow other users
- User should be able to retweet 
- User can like, comment on tweets
- Users should be able to favorite/bookmark a tweet
(Similar to Saved in Insta/FB)
- Create User Timeline (Top Tweets from All People a User Follows)
- User Profile Page: (Tweets/Re-Tweets Posted By User)
- Tweet can support: Pics, Videos, GIFs, Locations etc




(B) Non-Functional Requirements:

- Our Service should be highly available


- Minimum Latency for:

(1) News feed Generation: Most Customer Facing Feature (~< 100 ms)
(2) User Services: Profile Service (Changing DP, Username, Bio, Account Details etc)
(3) Search Service: Hashtags, keywords, Locations, Usernames



- HIGHLY Reliable
Any uploaded/provided content (photo/video) should not be deleted


- Availability > Consistency
(Consistency can take a hit in the interest of Availability)
(A user does not see a latest photo for some milliseconds, its acceptable)
(My services should be up 24*7)

- Eventual Consistency





(C) Extended Requirements:

- Adding Tags to Photos
- Tag People/Pages to Collaborate
- Searching Photos on Hashtags, locations, keywords etc

- Recommendation System:

(1) Content to Show (Tweets etc) - Feed Personalisation
(2) Users to Follow/Suggested People/Pages







Scale Estimation:
- Very Popular Services: Instagram/ FB/ Twitter/ Uber etc: 1 Bn+ Requests/ Month
- Medium Popular Services: Tinyurl, Figma etc: 500 Mn Requets/Month
- Less Popular Services: Pastebin, Imgur: 50 Mn Requests/Month





(3) Capacity Estimation/ Back of the Envelope Estimation

Read-Heavy or Write-Heavy?

- Read-Heavy for Feed Service, R/W: 1:1 for Chat Service
- Lots of Reads from Instagram as compared to new Posts
- R/W Ratio: 100: 1





(A) Traffic Estimates
(B) Queries Per Second
(C) Storage Estimates
(D) Cache Memory Estimates



(A) Traffic Estimates

Assumption,
- Total Users: 1 Bn
- Monthly Active Users: 800 Mn
- Daily Active Users: 200 Mn



------> Tweet Views:

On an average,
1 User visits their profile (2 times a day) and 5 times for user timeline


Each Page Contains: 20 Tweets


200 Mn * ((2+5) * 20) ~= 28 Bn Views/Day





------> Tweet Likes:

1 User likes 5 Tweets a day:

200 MN * 5 = 1 Bn Likes/Day





(B) Queries Per Second


On An Average, 
1 Person/Account: Posts 2 tweets per day


-----> Write Case: 

200 Mn * 2 ~= 400 Mn Writes/Day
400 Mn /24 * 3600 seconds ~= 4.8 K Writes or Uploads per sec


-----> Read Case:

100: 1 R/W Ratio:

100 * 4.8 K = 480 K Reads/sec




(C) Storage Estimates:


1 Tweet:

140 chars: 140 chars * 2 bytes/char = 280 bytes
Metadata: TweetID, timestamp, location ~= 30 bytes


1 Tweet Size ~= 280 + 30 ~= 300 bytes


Total Storage for 1 Day:
200 Mn * 300 Bytes ~= 60 GB/Day


Considering Media as well (Pics, Videos, GIFs etc)

1 out of 5 posts with a pic
1 out of 10 posts with a video


Photo: 200 KB (After compression)
Video: 2 MB (After Compression)


Size: (200 Mn/5 * 200 KB) + (200 Mn/10 * 2 Mb) ~= 48 TB/Day



Total Storage for 10 Years
= 40 TB * 365 Days * 10 Years ~= 142 PB





(D) Cache Memory Estimates


Following 80-20 Rule for Cache:
Cache 20% of my daya


To Store 20% of these requests:

0.2 * 48 TB/Day = 9.6 TB of Cache Memory/Day









(3) System APIs:

(A) POST API:

createTweet(user_uuid, tweet_data, tweet_location, media_ids[])

Parameters:

- user_uuid: Unique Identifier for User: String
- tweet_data: 140 chars limit tweet : String
- tweet_location: Given By User: lat, long
- media_ids[]: List of Medias (URLs)


Response:

2xx: Successful Post

{tweet_id: String

4xx: Client Side Error

5xx: Server Side Error






(B) GET API:

readTweet(user_uuid, tweet_id)


Parameters:

- user_uuid/XSRF Token: Unique Identifier for User: String
- tweet_id: Unique Identifier for Tweet



Response:

2xx: Successful Read

{status: "OK"}

4xx: Client Side Error

5xx: Server Side Error









(4) High Level Design


At High level, We Need to Support:

WRITE CALLS: 4.8 K Writes or Uploads per sec

READ CALLS: 480 K Reads/sec


At Peak Times:

- 100 K Write/Sec (20X Scale)
- 2-2.5 Millions (Reads/sec)



HLD Diagram: 
Image







(5) Defining Data Models:

- Type of Data to Store
- Choice of DB
- Mapping Across different Tables/Databases




-----> Observations:

We need to store:
- Users
- Their Posted Tweets
- People They Follow
- Favorite Tweets


Tweet Table:
- Store All Information Related to a Tweet 
(Likes, Comments, Retweets, Saves etc)



------> DB Schema:

Table-1: Tweet


- tweet_id: String: PK
- user_uuid: string: FK
- content: varchar
- tweetlat: int
- tweetlong: int
- creationdate: timestamp
- number_of_likes: int
- number_of_views: int
- number_of_rewteets: int
- media_file_path: URL (String/ varchar)



media_file_path: 
Location Storage of Photo/Content (S3 or Cassndra or HDFS)

Eg: 
Link: aws.cdn.amazon-us-east-2.link



Table-2: User


- user_uuid: int : PK
- Name: varchar
- Email: varchar



Table-3: UserFollow


UserID1: int ----- Composite Key
UserID2: int -----




POST


post/tweets/

{
	user_ID: 123,
	photos: 
	{
		a,b,c,d...... upto 20 photos
	}
}








-----> Choice of Databases

A straightforward approach for storing above schema
would be to use Relational DB like MySQL



Challenges:

- Partitioning will be challenging
- Querying would be very slow for billions of rows (clustered indexing)
- Not Easily Horizontally Scalable






Solution:

We can store above Schema in a Distributed Key-Value Storage
to have easier Horizontal Scaling


SQL to Key-Value DB Model Conversion:

PK ----> Key
Other Keys: Object ---> Value



Key: tweet_id 

Value: Object
{
	tweet_id: String: PK
	user_uuid: string: FK
	content: varchar
	tweetlat: int
	tweetlong: int
	creationdate: timestamp
	number_of_likes: int
	number_of_views: int
	number_of_rewteets: int
	media_file_path: URL (String/ varchar)
}





For Photos/Content:
- Distributed File Storage


Eg:
Amazon S3 or HDFS (Hadoop Distributed File Storage)


media_file_path: Link to S3/GHFS

media_file_path: 
Location Storage of Photo/Content (S3 or Cassndra or HDFS)


Eg: 
Link: aws.cdn.amazon-us-east-2.link












-------> Where to Store Relationship between Users and Tweets?


Wide-Column Database:
Eg: Cassandra



Key: User_ID
Value: [List of Photo_Ids]



Example:


UserID: 1123489393

PhotoID-1: 1123489393_1_17112024
PhotoID-2: 1123489393_2_17112024


Ref:


UserID 		| Tweet ID-1 			| Tweet ID-2 			|  Tweet ID-3  

1123489393    1123489393_1_17112024   1123489393_2_17112024   1123489393_3_17112024








-------> Why Wide Column Database:

- Cassandra or Any Other Wide Column Database DB always maintains a certain 
number of replicas by default
(To Offer Reliability)

- Read Operations are Extremely fast

- Deletions dont get applied instantly,
data is retained for sometime before permanent removal




CQL: 
Cassandra Query Language



Summary:

User Information: Relational DB/ SQL DB
Content: S3/ Blob
Relational b/w User-Content: Wide Column DB (Cassandra)
User-Follower: Composite Key: Relational DB/ SQL DB
Metadata: Likes, Comments, Shares, Location Tags - NoSQL DB




Wide Columnar DB:
- Cassandra
- Scyllia DB
- Google BigTable
- Microsoft CosmosDB
- Apache Hbase




Next:

- News Feed Generation
- News Feed Publishing
- Sharding/Partitioning
- Caching
- Ranking
- Load Balancing






At High Level,
We need to support 2 scenarios:

- Upload Photo/Content
- View/Search Photo/Content




Data:
- Actual Content (Pic/Video/Reel/GIF etc)
- Metadata about content


HLD Diagram:
Image - Phase-2







Uploading Content Service/Write Call Service is much slower
than Reading Content Service/Real Call Service owing to Cache etc





Problems with keeping a common service for both Read and Write:

(1) Much Higher Latency for Read Calls
(2) Can Block my system - bottleneck



Solution:

To avoid this bottleneck,
We use dedicated separate servers for read and write


This allows us to scale and optimise both read and write operations independently


Updated HLD Diagram:
Phase-2:






User -----> Read/Write ------> Read: App Server-1  ------> DB
							  Write: App Server-2: 







(5) Redundancy and Replication




--------> Redundancy (Duplication)

- Losing Files is NOT an option for my services - HIGHLY RELIABLE
- Store Multiple Copies of Each File ---- Redundancy
- If one db server crashes, we can pick photos from another db server



-------> Replication

To Ensure High Availability of My System,
We need to have multiple replicas of Database



Reason: Increase Availability of the System






Replica: 12:25 PM
1 File: 3 Copies


DB: 12:45
2 File: 
Each File: 3 Replicas

Main DB Instance Crashed
-----> Replica acts as DB Instance

1 File: 3 Copies
Redundancy ---> 2nd File: 3 Copies





Phase-3:
Detailed Architecture Diagram with Replication






(6) News Feed Generation


Newsfeed is generated from posts of users you are following 
(excluding suggestions/promoted posts)


Basic Understanding:

(1) Retrieve User_IDs of All Users that a particular User Follows
(2) Retrieve Latest, most popular and relevant posts for those User_IDs
(3) Rank these posts as per Users Interest
(4) Store in Cache and return Top Results (Like 50) to be rendered on 
a user homepage
(5) On the Client Side, When User Reached at the end of Current Feed (50 Posts)
They Can Fetch next 50 Posts


Achieve: By Pagination





-------> Fixed Size Timeline:

"You're all caught up" ----- Insta, Not on Facebook
https://about.instagram.com/blog/announcements/introducing-youre-all-caught-up-in-feed


-------> Infinite Timeline:

Facebook (2018)

Before 2018: Fixed Timeline




-------> Twitter:

Infinite Timeline


After Acquisition by Elon:
Rate Limiter: 
- Verified (Subscription, Blue Tick) : 6000 posts/day
- Non-Verified: 600 posts/day





Ranking:

Goal: Maximise User Engagement with the Content


User A -  follows 1000 Other Users
We need to generate Newsfeed and Rank it for User A



Degree of Closeness/Interaction with Another User:

- Likes
- Comments
- Shared
- Save
- Mention in Stories/Captions/Comments
- Chat
- Watch Time


User A follows B and C


A----B: Interaction Score: 80
A----C: Interaction Score: 60


If I show post from B and C to User A,
User A is more likely to Engage with post from User B.












--------> How to Generate NewsFeed:



2 Ways are There:
(1) Online/On the Go for NewsFeed Generation: Run Time Generation
(2) Offline Feed Generation


Zomato:

Open ----> Take Some Time to Load
- Generated at Run Time
- No Stale Data 
- Need Real Time Data Here


Instagram:

Open ----> Does Not Take Any Time to Load
- Feed is Already Ready
- Generated Offline
- Stale Data
- No Need to Real Time Data






(A) On the Go Newsfeed Generation/ Run Time Load:

There are issues with On the Go News Feed Generation:

(1) Latency, We need to Generate homepage/timeline instantly
(2) For Users who follows a lot of other users (large following - not followers),
we need to perform sorting/merging/ranking for a huge number of posts ------- VERY VERY SLOW
(3) For Live Updates, Each Update will result in creating new query to get the most updated result
(4) For Celebrity Users (Many Followers: > 1 M), live update would have very very 
heavy loads to push for new content to all their followers







(B) Offline Feed Generation:


Structure:
User_ID: {pre-generated feed items}


Eg:

User123: 
{
	feed1: {name: , content: , duration: }
	feed2: {name: , content: , duration: }
}




- We have dedicated service for offline newsfeed generation
- Continously generate user feed and store in memory (Top 500), Cache: Top 50: Pagination
- Whenever a user logs into their feed,
Immediately serve the pre-generated/pre-computed feed ----- STALE Data
- For Subsequent Requests -----> Run the dedicated newsfeed generation service again

(Regular Intervals: Have a lookup for new Items in the Feed)




For New Updates:

(1) Server -------> Client Communication
SSE

"New Posts \|/": Linkedin
Automatic Refresh: Instagram


(2) Client -----> Server Communication
Pull/Refresh the App Screen




Pagination:

1000 Response ----> 50 Responses x 20 based upon NextToken







--------> How Many Feed Items to Store in Memory for Offline Feed Generation a User?



Initially, we can decide 500 Items per User for Offline Feed Generation
But, we can change as per usage pattern of the user.



Paging: 50 Posts per Page


On an Average,
User Scrolls: 10 Pages
Total Posts: 500


Optimisation: 

(1) Based upon User Behaviour, User_ID: {pre-generated feed}
(2) Avoid Generating News feed for Inactive Users



User           Following         Followers

User-1         1000              1000000
User-2         500               5000


User_ID: {pre-generated feed}

Pattern:

User-A: Is Most Active at 7 PM on {Days}
User-B: Is Most Active at 11 PM on {Days}












----------> Should we generate (and store in memory - CACHE) newsfeed for ALL USERS?

DAU: 1 Bn
MAU: 2 Bn+


(1) Users which do not login frequently, 
we dont need to generate actively newsfeed for the,


(2) Users:

- Passive Users: Not Very Frequent 
- Active Users: Very Frequent, Atleast once in a day



2 Ways:

(1) LRU Based Cache:
Remove newsfeed for Users that you have not accessed newsfeed for a long time

(2) Optimised Way:
Find the login pattern of the active users


- What time of the day: Most Active
- Which days of the week: Most Active









(7) News Feed Publishing


Process of pushing a post to all followers in called - FANOUT

There are 2 Approaches:

- Pull Model/ Fan Out on Load
- Push Model/ Fan Out on Write




(1) Pull Model/ Fan Out on Load


Clients can pull data on a regular basis whenever they need it.


Problems:

(1) Client Must Request to Get New Data, Server Itself is NOT Pushing Data to Client
(2) Majority of the Pull Requests will lead into Empty Responses: AJAX Call/ Short Polling


(2) Push Model/ Fan Out on Write

Once a User published a post,
we can immediately push this to ALL Followers



Advantage:
Make my system less read-heavy because now the Users
does not need to fetch everytime for AJAX Call/ Short Polling




Usual Flow:

In Pull Model,
User -----> Check for all Following UserIDs -----> Check for most updated content:
													Too many read calls

Users Maintain (Client) Short Polling with the Servers: READ-HEAVY



In Push Model,
Updated data is sent to the user followers without asking for Read Calls

-----> Saved Many Read Operations

Users Maintain (Client) Long Polling with the Servers: WRITE-HEAVY



Disadvantage:

(1) For Celebrity Users (> 1 M Followers)
Server will need to push updates to Million+ Accounts: WRITE HEAVY

(2) For Inactive Users:
Push Down is a waste of resource





(7) News Feed Publishing


Process of pushing a post to all followers is called - FANOUT

There are 2 Approaches:

- Pull Model/ Fan Out on Load
- Push Model/ Fan Out on Write




(1) Pull Model/ Fan Out on Load


Clients can pull data on a regular basis whenever they need it.


Problems:

(1) Client Must Request to Get New Data, Server Itself is NOT Pushing Data to Client
(2) Majority of the Pull Requests will lead into Empty Responses: AJAX Call/ Short Polling


(2) Push Model/ Fan Out on Write

Once a User published a post,
we can immediately push this to ALL Followers



Advantage:
Make my system less read-heavy because now the Users
does not need to fetch everytime for AJAX Call/ Short Polling




Usual Flow:

In Pull Model,
User -----> Check for all Following UserIDs -----> Check for most updated content:
													Too many read calls

Users Maintain (Client) Short Polling with the Servers: READ-HEAVY



In Push Model,
Updated data is sent to the user followers without asking for Read Calls

-----> Saved Many Read Operations

Users Maintain (Client) Long Polling with the Servers: WRITE-HEAVY



Disadvantage:

(1) For Celebrity Users (> 1 M Followers)
Server will need to push updates to Million+ Accounts: WRITE HEAVY

(2) For Inactive Users:
Push Down is a waste of resource





(3) Hybrid Model:

Combination of Pull Model + Push Model


(A) Normal Users (Few Thousands of Followers)
- Use Push Model
- For People with few thousands followers, 
just use the server to send the most updated content 


(B) Celebrity Users (> 1 M Followers)
- Use Pull Model
- Users/Followers can pull the data on a regular basis whenever they need it.




Customer:

User:
500 Following

450: Normal Users
50: Celebrity Users


When you Open Your Feed?
- There would be NO Post from Celebrity Users,
Only posts from Normal Users



1st Case: Pre-Generated Feed
No New Celebrity Post

Refresh: New Feed Generation
- Top: Celebrity User Post
- Add The Celebrity User Post







1st
2nd
3rd



Bangalore, India
1st and 2nd Degree Connections -----> See the Post (10% of them)

Reactions in 't' hours (3 Hrs) 
----> Shown to next 10%, next 10%

Then, Expand to Other Geolocations based upon Reactions













---------> How Many Feed Items to return in each request?


In 1 request, 
we can have 50 Items


Depends on Client Device (Mobile vs Desktop)

IMP:

Client Devices:

- Laptop:
- Mobile: Data Usage is Very Expensive (When on Mobile data)



Mobile: Wifi: Consume More Bandwdith
Mobile: Mobile Data : Consume Less Bandwdith


-------> Bitrate Adaption



-------> Example-1: Instagram

Use Case: 
See the Instagram News Feed on 2 Devices

Case-1:

Mobile Connected to a Very High Speed Wifi (Higher Bandwdith)
Default Size for fetching newsfeed = 50


Case-2:

Mobile Connected to a Very Low Speed Mobile Data (Lower Bandwdith)
Default Size for fetching newsfeed = 20



Prefer to show less number of posts on newsfeed in shorter time
vs
More number of posts on newsfeed in higher time


50 posts -> 5 sec VS  20 posts -> 1 sec (Preferred)














-------> Example-2: Youtube


Two Users are Watching the Same Video

User-A: Mobile: Wifi: 5G
Video: Load in 720P


User-B: Mobile: Mobile Data: 3G
Video: Load in 360P














(8) Feed Ranking:


Factors Contributing to Ranking in Feed:

Degree of Closeness/Interaction with Another User:

Ranking Score:

- Likes
- Comments
- Shared
- Save
- Tagged in Stories/Captions/Comments
- Chat
- Watch Time
- Users Interest


User A follows B and C


A----B: Interaction Score: 80
A----C: Interaction Score: 60


If I show post from B and C to User A,
User A is more likely to Engage with post from User B.










(9) Data Partitioning/ Sharding


For My Newsfeed Generation,
Use can use DB Sharding



Basis of Sharding/ Partition Key:
User_ID or Media_ID



Problem with Media_ID based Sharding:

(A) 1 User can posts a lot of content


Profile Page: All Contents posted by the User


Go to All Partitions/Shards, merge the responses for that particular user
and return ----- VERY SLOW


(B) Too many people can post the same photo 

- Viral Posts: 
Lead to Uneven Distribution of Load


Media_ID         Views/Read Requests:

M_123                   10
M_456 (Viral)           10 M




Solution:
Keep Sharding/Partitioning Key as User_ID


Advantage: 
Get All Information for a User under 1 Partition,
No Need to Aggregate responses from multiple Partitions


















(10) Caching:

- Which Cache (Why?)
- Size of Cache
- Which Cache Eviction Policy
- Handle Cache Invalidation (Cache Miss Case)


Cache:

We need a very highly scalable content delivery system across the globe.
Our service should be able to push its content closest to the servers distributed globally.
Hence, we use CDN.


----> Which Cache

Cache: 
Cache the Cotnents that are frequently used


Eg: Redis, Memcache, Hazelcast


The Application Server before hitting the DB Storage ----> Quickly check in Cache







------> Size of Cache:


Following 80-20 Rule for My Cache:
Cache 20% of my data


To Store 20% of these Requests:
0.2 * 400 TB/day  = 80 TB of Cache Memory/Day





------> Which Cache Eviction Policy?

When the cache is full,
and we want to replace a content with a newer content


LRU can be a good solution,
Discard the Least Recently Used URL first and store the new URL







-----> How can each replica of cache can be updated?

In Case of Cache Miss,
Request will go to DB


When this happens,
After retrieving from DB, we can update the cache
and pass the new entry to all replicas.



- Each replica can update its cache by adding the new entry












(11) Load Balancers



Where do we Add Load Balancers?

(1) Between Client and Application Servers (Common)
(2) Between Application Server and DB Server (Common)
(3) Between Application Server and Cache Server (Rare, Only for Large Systems)



--------> Which Policy to use for Balancing the Requests in Load Balancers?


(1) Use Round Robin:

Adv:

- Equally distributed load among servers
- If 1 server is not working, LB will move the next available server


(2) Problem with Round Robin?
- Load Amount/Health/Response Time on a Server is not taken care


10 Req - Server-1: 10 sec

10 Req - Server-2: 15 sec


Req No: 21 ----> Ideally go to Server-1 -> Round Robin will not guarantee that



Better Solution:

Predictive Analysis for Load Balancing/Smart Load Balancers

LB Solution can be periodically queried to know the health and load of DB Servers


Eg:

Netapp, Nginx, ELB






(12) Analytics/Telemetry: EXTENDED




Metrics:

- Country of Visitors
- CTR
- Date and Time
- No of Views


Architecture:
EDA / Event Based or Driven Architecture

Generate Kafka Event everytime a URL Re-Direction happens
Put that Event in an EQ (Event Queue) for Analytics

On Top of that, 
Run Apache Spark and Hadoop Queries to get more insights









Note:

To Avoid Duplication:

Media_ID: M_123 is posted by 1000 Users


Does it make sense to upload the same Media 1000 Times on Instagram Server?

S3:
File Upload -----> HashCode

New File to Upload,
Check HashCode is Present ------> Reuse the previous HashCode
Else, Upload it.





Final Design:
Phase-3








Final Diagram:
[Twitter]
























--------> Case Study-6: Designing Cloud Based File Storage System - Dropbox/Google Drive/One Drive/Apple Drive


- File Hosting (Storing) and Sharing Service
- Cloud Hosting Service:
Users can upload their data/files to cloud from their remote machines
(Eg: Pushing Code to Github from Local)
- Data should be automatically synced across ALL Devices in Real Time


DEMO:
drive.google.com

























