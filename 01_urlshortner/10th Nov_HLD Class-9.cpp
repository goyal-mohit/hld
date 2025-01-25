Date : 10th Nov 2024
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

To scale out or DB, we need to Partition it so that we can store information
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
Based Upn Key ----> Partition Number



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

20K Read Requests/second 8 3600 * 24 = 1.7 Billion Read Requests/Day

Following 80-20 Rule for My Cache:
Cache 20% of my data


To Store 20% of these Requests:
0.2 * 1.7 Billion * 500 bytes/URL = 170 GB of Cache Memory



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
(2) Mark that short url key ----> Available to resuse
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


Allow Users to store textual contentand generate a short Unique URL to share it.



































































