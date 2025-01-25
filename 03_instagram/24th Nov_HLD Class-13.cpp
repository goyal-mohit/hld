Date : 24th Nov 2024
Mentor: DEVANG SHARMA
Batch: Sept Batch (DSA) - Tutort Academy

Agenda: Basics of System Design

- Basic of System Design: DONE
- Scalability: DONE
- Availability: DONE
- Consistency: DONE
- Load Balancing: DONE
- Caching: DONE
- CDN: DONE
- CAP Theorem: DONE
- Choice of DB: DONE

- Data Partitioning: WIP
- Reliability
- Efficiency
- Manageability
- Resiliency
- Polling (Long/Short) vs Web Sockets vs SSE  
- Data Partitioning
- Indexing
- Redundancy and Replication
- Consistent Hashing
- Proxies and Types, Proxies vs Gateways
- 10 Case Studies: Whatsapp, Instagram, Yelp, Uber, Dropbox, Google Drive, URL Shortner, Netflix

"Please Type 'Hi' in the Chat Box if you have joined and Can See this Screen".








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
No Celebrity Post

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






















--------> Case Study-4: Designing Twitter (X)


Twitter:
https://github.com/twitter/the-algorithm/tree/main




Twitter:

- Social Media Platform where users post and read short posts (140 chars) called "Tweets"
- Tweet can support: Pics, Videos, GIFs, Location etc
- Non-Registered Users: Can just read the tweets (Private Tweet vs Public Tweets)
- Registered Users: Reaf and Write (Like, Comment, Retweet, Share)
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






























