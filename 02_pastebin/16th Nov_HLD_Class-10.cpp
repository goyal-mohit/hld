Date : 16th Nov 2024
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







---------> Case Study-2: Designing a Text Sharing Service like Pastebin

Pastebin = Tinyurl (URL Shortner) + Data (Text)

Pastebin:
- Web Service where users can store and share textual content (plain text, Software Licenses, Source Code etc)


Similar Services:
pasted.co, chopapp.com



(1) Demo:
https://pastebin.com


Eg:
https://pastebin.com/ia5pmyX0

- Unique Link for Every Paste
- Link can be Public/Unlisted/Private
- API/Tools/Integration
- Analytics/Telemetry
- Expiry Time: Can be Set by User


Allow Users to store textual content and generate a short Unique URL to share it.







(2) Requirements Clarification:

(A) Functional Requirements

- User should be able to upload or paste their textual data and get a 
unique URL to access it.
- Paste: Only Textual Data (.txt, .json etc)
- Only able to upload textual data
- Each data and link will have a default timespan before it expires (5 Years)
- Provides Option to User to delete the Paste
- Shortened Pastebin URL should be Unique
- Max Content Size: 10K Chars
- Upload Service: Limit on Size of File to be uploaded: 10 KB
- Formatting, Languages Support, Syntax Highlighting
- Author can update the edit (After Sign In)



(B) Non-Functional Requirements:

- System should be HIGHLY Available
Because, If our System is Down, All Pastebin Re-Directions will not Happen
- System should be HIGHLY RELIABLE
ANY data uploaded/provided should not be lost
- URL Redirections should happen in real-time with minimum latency



(C) Extended Requirements:

- Users should be able to pick a custom URL
- Users can mention the expiry time of short URL
- Analytics: Redirections: Clicks, Views, CTR (Click Through Rate), Audience etc
- Third Party Integratins: REST API to Other Systems














Scale Estimation:
- Very Popular Services: Instagram/ FB/ Twitter/ Uber etc: 1 Bn+ Requests/ Month
- Medium Popular Services: Tinyurl, Figma etc: 500 Mn Requets/Month
- Less Popular Services: Pastebin, Imgur: 50 Mn Requests/Month





(3) Capacity Estimation/ Back of the Envelope Estimation

Read-Heavy or Write-Heavy?


- Read-Heavy
- Lots of Re-Directions requests compared to new URL Shortenings
- R/W Ratio: 10: 1


(A) Traffic Estimates
(B) Queries Per Second
(C) Storage Estimates
(D) Cache Memory Estimates




(A) Traffic Estimates:

Assumption,
1 Million paste to our system on daily basis


(B) Queries Per Second

- Read/Write Ratio: 10: 1

No of Write Calls in a second = 1 Mn/24 Hrs * 3600 sec ~= 12 pastes/sec

No of Read Calls in a second = 10:1 Ration ~= 120 reads/sec




(C) Strorage Estimates


Assumption:
Every Paste is stored for 5 Years/ Expires after 5 Years

Limit: 
Users can upload maximum of 10KB of data

Why?
- Data stored is ONLY Textual Content: Source Code, Configs, Logs

Average Storage Estimates per day:
1 Mn * 10 KB ~= 10 GB/Day

Total Number of Links/Paste to Store:

30 Million URLs every Month * 12 Months * 5 Years = ~= 1.8 Billions Pastes



Content Size = 10 KB
Additional Details: user_uuid, location, creation_date, expiry_time, metadata etc


Total Size for 1 Paste = 10 KB + 10 KB = 20 KB


Total Storage Required in 5 Years
= 1.8 Billions Paste * 20 KB ~= 36 TB










(4) System API:



(A) Request:

createPaste(user_uuid, content, expiry_time)
POST/WRITE Request


Parameters:

user_uuid: Unique Identifier for a user: STRING
content: Textual Data/File: String
expiry_time: By Default: 5 Years, 
Extended Requirements: Might be able to change



XSRF Token: User not logged in
user_uuid: User logged in





Response:

Returns the Short URL to the Paste: STRING



Success:
Return the Short Unique URL to the Paste which is MAPPED to the content you paste/uploaded


Non-Success:
Error Code










(B) Request: 
deletePaste(user_uuid, pastebin_link)

DELETE/WRITE Request


Parameters:

user_uuid: Unique Identifier for a user: STRING
pastebin_link: Unique Short URL to the Paste: STRING



Logic:

if (user_uuid == admin_id || user_uuid == author_id)
	set permission = true;


Response:

- Successfully Deleted: String


Success:
Successfully Deleted


Non-Success:
Error Code












(C) Request:
getPaste(user_uuid, pastebin_link)

Parameters:

user_uuid: Unique Identifier for a user: STRING
pastebin_link: Unique Short URL to the Paste: STRING


Response:
- Web Page


Logic:

if (user_uuid == admin_id || user_uuid == author_id || user_uuid == whitelisted_id)
	set viewPermission == true;





Success:
Successfully Loaded Webpage

Non-Success:
Error Code





URL - Textual Content
1:1 Relationship











-----> How do we avoid abuse/malicious practise?


Abuse Case:

- Someone generating infinite Paste

- Each Short URL: 6-8 characters: Limit for number of paste,
Evnetually exhaust all possible combinations


- No Other Shortening Possible


Solution:

Rate Limiting/ Throttling for user_uuid


Cyber Security:

- XSS
- DOS
- DDOS
- Man In Middle

















(5) Defining Data Model:


- Type of Data to Store
- Choice of DB
- Mapping Across different Tables/Databases



------> Observations:

Part-A: Short URLs

(1) We need to store billions of records
(2) Each Object we store is small (~500 Bytes)
(3) There are NO Relationships between Records, 
apart from which User has created which URL
(4) Our Service is READ-HEAVY


Part-B: Actual Textual Content Mapped to the Short URL

(1) Object Storage: Maximum of 10 KB per Object (Text/File)
(2) Our Service is READ-HEAVY




-----> DB Schema:


Table-1: Paste

- pastebin_link: varchar: PK
- content_id: varchar (Mapped with pastebin_link)
- creation_time: timestamp
- expiration_time: timestamp
- user_uuid: FK


Table-2: User

- user_uuid: Int: PK
- name: varchar
- email: varchar



Note:

Each pastebin_link would be Unique and Short URL which would map to a unique content_id






------> Choice of Database?

(1) For ShortURL:

We anticipate storing Billions of Records,
and We Dont Need to Use Relationships between Tables

Ideal DB:
NoSQL DB


Eg: Mongo DB, S3 etc



(2) For Content:

We anticipate storing Billions of Records,
with each object max size: 10 KB

We Dont Need to Use Relationships between Contents



Ideal DB?
Object Storage/ Blob Storage


Eg: Amazon S3


key: content_id
value: Object/Blob




End to End Journey:

User Upload a File -----> AWS S3 Bucket  -----> Link Generated ------> Mapped to the Paste Table
-----> Mapped to the User Table



Link: aws.cdn.amazon-us-east-2.link


Storage:

Key: aws.cdn.amazon-us-east-2.link: content_id
Value: file.txt











------> High Level Diagram





(6) Algorithms:

Major Part: How do you generate a short AND Unique URL?

short: 6-8 characters


(A) Encoding Actual URL - Hashing


- We can compute a unique hash of the given URL
That Hash can be encoded for displaying


- Algos:
MD5, SHA256, SHA512, Base64 etc



Display in Short URL:
- Alphanumeric Values


a-z: 26 characters
A-Z: 26 characters
0-9: 10 characters

Total: 62 characters


Size: 6-8 characters


Short URL: 6 characters
Total Combinations = _ _ _ _ _ _ = 62^6 ~= 60 Bn Strings/URLs


Short URL: 8 characters
Total Combinations = _ _ _ _ _ _ _ _ = 62^8 ~= 260 Trillions Strings/URLs

For Our Use Case:
For 5 Years, We need 30 Billion URLs
Hence, we are good with 6 chars





------> Issue?


(1) If Multiple Users Enter the Same URL, They get Same Shortened URL


Because: MD5/SHA256/SHA512 will create the same Hash for same input for differnet users


Need:
Same Input from Diff Users -----> Diff Short URLS

Website: google.com

User-A, User-B


User-A -----> SHA256 (google.com) ----> xyzabc

User-B -----> SHA256 (google.com) ----> xyzabc



(2) System ------> Long URL -----> Hashing/Encoding -----> Short URL (6 chars)

- Extra Latency introduces while encoding
- Hashing is generated "On the Go/On the Fly" (Generated in Run Time)





---------> Workaround?

Problem:

(1) If Multiple Users Enter the Same URL, They get Same Shortened URL


Solution:

(A) Add user_uuid to make it unique
You need to use users information in Short URL

Logged In: user_uuid
Not Logged In: XSRF Token 


OR


(B) I can add a Counter/Sequence Number to make It Unique

google.com ----> https://tinyurl.com/abcda1

google.com ----> https://tinyurl.com/abcda2

google.com ----> https://tinyurl.com/abcda3

google.com ----> https://tinyurl.com/abcda4

For Very Lage Number of Shortenings:
Sequence Number will be Very Large



HLD Diagram for Using Encoding Method:
- [Image]











(2) Solution: Generate Keys (Short URLs) Offline

Problem:

System ------> Long URL -----> Hashing/Encoding -----> Short URL (6 chars)

- Extra Latency introduces while encoding
- Hashing is generated "On the Go/On the Fly" (Generated in Run Time)




Solution:

Have a KGS (Key Generation Service) that generates random 6 characters beforehand
AND store them in a database (Key-DB)

Whenever a user wants to shorten a URL,
Just take one of the pre-generated keys and Use it.


Advantages:

- Extremely fast (Not creating on the go, pre-generated keys)
- No need to worry about duplicates
- No Collision
- KGS will make sure that all the Keys that are inserted into Key-DB are Unique






------> What About Concurrency?

As soon as a key is used,
It should be marked in the Key-DB to ensure, its not reused

If there are Multiple Servers reading keys concurrently, 
We Might have case where 2 or more servers try to read the same key from DB


Key-DB:

key-1  <------- Server-1, Server-2
key-2
......
......
......
......



Solution:

(1) Assign the Range of Keys to Servers

Key-1 to 1 Mn: Server-1
Key-1 Mn to 2 Mn: Server-2


(2) Keep the KGS Synchronized

Server can use KGS to read/mark they keys in the database

As soon as a key is used, flag the key as unavailable

Use a Flag:
isAlreadyUsed: true/false


(3) Introduce Row Level Locks
Synchronized Block and Assign Locks on Read

While Server-1 is Write, Nobody else can Write, They have the Read Access

FCFS.






------> Size of Key-DB:

6 Characters per Key * 60 Billion Unique Keys = 400 GB


------> Is KGS a Single Point of Failure?

Yes, It is.


Solution:

We can create replicas/standby/secondary of KGS
Whenever the primary service is down, standby service take over to generate and provide keys






-------> How to perform key lookup?

We can lookup the key in our database to get the full URL

If its present in the DB,
Send the Stored URL (Long URL)



https://www.google.com -------> https://tinyurl.com/76376736

https://tinyurl.com/76376736 -------> https://www.google.com



HLD Diagram:
Phase-3 [Image]








(7) Data Partitioning and Replication

To scale out our DB, we need to Partition it so that we can store information
about Billions of URLs



--------> What should be the Partitioning Schemes?

(1) Range Based Partitioning

Short URLs: 6-8 Characters

- We can store URLs in separate partitions based upon the first letter of the 
Short URLs

- Save All URLs starting with letter 'a' ---- partition: 1
- Save All URLs starting with letter 'b' ---- partition: 2


a-z: 26
A-Z: 26
0-9: 10
Total: 62

Total Partitions: 62


Problem:

- Unbalanced Partitioning/ Uneven Load Distribution


Eg:

partition-1: starting with 'a' has more short URLs: HIGH LOAD

partition-2: starting with 'b' has less short URLs: LESS LOAD






(2) Hash Based Partitioning

We take hash of objects we are storing
Then calculate which partition to use based upon hash


In Our Use Case:

Short URL ----> Hash -----> Key
Based Upon Key ----> Partition Number



Eg:

hash() = key % 256 (any constant)

Hash Upto 256 Values: 0 to 255

256 Partitions -----> Follow Round Robin Approach


req-1: 1st Short URL: DB Partition-1
req-2: 2nd Short URL: DB Partition-2
....................................




Replication of Partitions:

To Avoid any point of failure,
We can replicate each partition












(8) Caching

- Which Cache (Why?)
- Size of Cache
- Which Cache Eviction Policy
- Handle Cache Invalidation (Cache Miss Case)




----> Which Cache

Cache: 
Cache the URLs that are frequently used


Eg: Redis, Memcache, Hazelcast


The Application Server before hitting the DB Storage ----> Quickly check in Cache


Diagram:
Phase-3





------> Size of Cache:

120 Read Requests/second * 3600 * 24 = 10 Million Read Requests/Day

Following 80-20 Rule for My Cache:
Cache 20% of my data


To Store 20% of these Requests:
0.2 * 10 Millions * 20 KB/paste = 400 GB of Cache Memory



Server: 512 GB/ 1 TB
1 Server is Sufficient






------> Which Cache Eviction Policy?

When the cache is full,
and we want to replace a link with a newer link


link-1: 5 Times: Last Used: 24 Hrs Ago: LFU
link-2: 10 Times: Last Used: 1 Year Ago: LRU


LRU can be a good solution,
Discard the Least Recently Used URL first and store the new URL







-----> How can each replica of cache can be updated?

In Case of Cache Miss,
Request will go to DB


When this happens,
After retrieving from DB, we can update the cache
and pass the new entry to all replicas.



- Each replica can update its cache by adding the new entry



Diagram:
Phase-3






(9) Load Balancers


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





Final Diagram:
Phase-4









(10) Purging or DB Cleanup Service

Do I need URL Entries Forever?
- No


Should I check daily or actively for expired links? (Eager/Quick Cleanup)
- Put load on my DB server


Instead,
We can slowly remove the expired links and do a lazy cleanup of DB.
Our Cleanup Service will make sure that only expired links are deleted




Possible Scenarios:

(1) When a user tries to access an expired link


google.com -----> tinyurl/abc


After 5 Years,
tinyurl/abc has expired

(1) delete the short url key
(2) Mark that short url key ----> Available to reuse
(3) Response to Client: URL Expired



(2) A cleanup service can run periodically to remove the expired short URLs
from DB and Cache


Service should be VERY Lightweight (Less Resource Intensive)


Run Only when the user-traffic is at Non-Peak Hours

(3) Default Expiration Time: 5 Years

(4) After removing a short URL Key,
We can put it back in the Key-DB to be reused.


Non-Peak Hours:

Monitoring Tools:

Grafana, Kibana, Dynatrace, AWS Metrics


Steps:

1 - Delete Short URL from Main DB 
2 - Delete Short URL from Cache
3 - Mark Short URL as available to reuse in Key-DB








(11) Analytics/ Telemetry: EXTENDED


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






(12) Security and Permissions: EXTENDED


Can Users create a custom URL?
- Yes


Can Users create a private shortened URL for particular group of users ONLY?
- Yes


Solution:

You can use Permissions levels (Private/Public) of URL 
along with URL in DB

private: Specific Set of Users -----> Whitelisted them (based upon email/numbers)


For Custom Short URL,
Check the (isPremiumMember == true)



Final Diagram:
Phase-4





















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
- User can upload stories (Valid: 24 Hrs) - Open to All Follwers, Close Friends (Green Icon)
- Instagram Live


Additional:
- Search Content based upon hashtags and keywords and location
(Included from Captions and Tagged Locations)

EG: California Bridge




(B) Non-Functional Requirements:

































































