Date : 17th Nov 2024
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







-------> Case Study-3: Designing Instagram


Instagram:
- Social Media with Photo (Videos/GIFs etc) sharing service
where users can upload photos to share with others



Similar Services:
Facebook, Picasa etc




Demo:
https://instagram.com/



A Social Media Platform where a user is able to:
(A) Public Account: Anyone can see
(B) Private Account: Selected Users can see

- Upload Content (Reels, GIFs, Pics, IGTV etc)
- Share:
- Comment
- Like
- News Feed Generation
- Homepage/Profile Page/Timeline
- Chat: 1:1 Chat, Channels (Only Admin), Group Chats
- Follow Other Users (Bi-Directional Graph)
- Posts Stories 
- Calls - Audio, Video





(1) Requirements Clarification:

(A) Functional Requirements:

- User should be able to upload and view photos/content
- User can follow other users
- Users can search based upon hashtags and keywords
- System should be able to generate a users News Feed which consists of
top photos/recent from all the accounts a user follows
- User can have a Homepage/Timeline/Profile page consisting of all content posted by user
- User can have chat: 1:1 Chat, Channels (Only Admin), Group Chats
- User can upload stories (Valid: 24 Hrs) - Open to All Followers, Close Friends (Green Icon)
- Instagram Live


Additional:
- Search Content based upon hashtags and keywords and location
(Included from Captions and Tagged Locations)

EG: California Bridge




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
- Add music to posts

- Recommendation System:

(1) Content to Show (Reels/Pics etc) - Feed Personalisation
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
- Total Users: 3.1 Bn
- Monthly Active Users: 2 Bn
- Daily Active Users: 1 Bn




On An Average, 
1 Person/Account: Posts 2 pic per day




- Read/Write Ratio: 100: 1


------> No of write Calls in a Day:

1 Bn * 2 ~= 2 Bn Writes/ Day



------> No of Read Calls in a Day:

100: 1 Read/Write Ratio ~= 200 Bn Reads/Day






(B) Queries Per Second



-----> Write Case: 

2 Bn /24 * 3600 seconds ~= 24K Writes or Uploads per sec


-----> Read Case:

100: 1 R/W Ratio:

100 * 24K = 2.4 Mn Reads/sec




(C) Storage Estimates

Average Photo Size: 200 KB (After Compression)

Compression/Encoding Algo: RLE (Run Length Encoding)

Average Content Size: 20 MB (After Compresion) (Including Videos, Reels etc)

Total Storage for 1 Day:
1 Bn * 20 MB * 2 Pics ~= 400 * 100 TB/Day ~= 40 PB/Day

10^9 * 10^67 = 10^16
1 TB = 10^12 Bytes
1 PB = 10^15 Bytes

Total Storage Required for 10 Years
= 40 PB * 365 Days * 10 Years = 143000 PB ~= 143 ZB




(D) Cache Memory Estimates


Following 80-20 Rule for Cache:
Cache 20% of my data

To Store 20% of these Requests:
0.2 * 40 PB/Day = 8 PB of Cache Memory per Day










(3) High Level System Design



At High Level,
We need to support 2 scenarios:

- Upload Photo/Content
- View/Search Photo/Content




Data:
- Actual Content (Pic/Video/Reel/GIF etc)
- Metadata about content


HLD Diagram:
Image











(4) Defining Data Model:


- Type of Data to Store
- Choice of DB
- Mapping Across different Tables/Databases



------> Observations:

We need to Store:
- Users
- Their Uploaded Content
- People They Follow



Photo Table:
- Store All Information Related to Photo







------> DB Schema:

Table-1: Photo

- PhotoID: int: PK
- UserID: int FK
- PhotoPath: varchar
- PhotoLat: int
- PhotoLong: int
- UserLat: int: Current Location
- UserLong: int: Current Location
- Creation date: timestamp




Table-2: User


- UserID: int : PK
- Name: varchar
- Email: varchar




Table-3: UserFollow


UserID1: int ----- Composite Key
UserID2: int -----




PhotoPath: 
Location Storage of Photo/Content (S3 or Cassndra or HDFS)


Eg: 
Link: aws.cdn.amazon-us-east-2.link





UserID1 -- UserID2

UserID1 ---> UserID2
UserID2 ---> UserID1




PhotoID: Unique


= User_ID + Incrementaing Sequence + Timestamp

User_ID: 1123489393

Photo-1: 1123489393_1_17112024
Photo-1: 1123489393_2_17112024



Check in Instagram:

instagram.com/posts/{username}/id_1/2/3



POST


post/photos/

{
	user_ID: 123,
	photos: 
	{
		a,b,c,d...... upto 20 photos
	}
}







--------> Choice of Database:


A straightforward approach for storing above schema
would be to use Relational DB like MySQL



Challenges:

- Partitioning will be challenging
- Quertying would be very slow for billions of rows (clustered indexing)
- Not Easily Horizontally Scalable






Solution:

We can store above Schema in a Distributed Key-Value Storage
to have easier Horizontal Scaling


SQL to Key-Value DB Model Conversion:

PK ----> Key
Other Keys: Object ---> Value





Key: PhotoID: 

Value: Object
{
	UserID: int FK
	PhotoPath: varchar
	PhotoLat: int
	PhotoLong: int
	UserLat: int: Current Location
	UserLong: int: Current Location
	Creation date: timestamp
}





For Photos/Content:
- Distributed File Storage


Eg:
Amazon S3 or HDFS (Hadoop Distributed File Storage)


PhotoPath: Link to S3/GHFS

PhotoPath: 
Location Storage of Photo/Content (S3 or Cassndra or HDFS)


Eg: 
Link: aws.cdn.amazon-us-east-2.link












-------> Where to Store Relationship between Users and Photos?


Wide-Column Database:
Eg: Cassandra



Key: User_ID
Value: [List of Photo_Ids]



Example:


UserID: 1123489393

PhotoID-1: 1123489393_1_17112024
PhotoID-2: 1123489393_2_17112024


Ref:


UserID 		| Photo ID-1 			| Photo ID-2 			|  Photo ID-3  

1123489393    1123489393_1_17112024   1123489393_2_17112024   1123489393_3_17112024








-------> Why Wide Column Database:

- Cassandra or Any Other Wide Column Database DB always maintains a certain 
number of replivas by default
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
Image - Phase-1







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




--------> Redundancy

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



















