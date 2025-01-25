Date : 23rd Nov 2024
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







A -----> C <----- B


A: SQL
B: NoSQL


C: NoSQL

Aggregator Service:

Dump: SQL + NoSQL

Parse SQL ----> JSON


Data Sources Connector: 
- Snowflake
- AWS
- Google





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

-----> Saved Many Read Operationss

Users Maintain (Client) Long Polling with the Servers: WRITE-HEAVY



Disadvantage:

(1) For Celebrity Users (> 1 M Followers)
Server will need to push updates to Million+ Accounts: WRITE HEAVY

(2) For Inactive Users:
Push Down is a waste of resource














