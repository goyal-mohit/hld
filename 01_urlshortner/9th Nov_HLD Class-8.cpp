Date : 9th Nov 2024
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






----------> Case Study-1: Designing a URL Shortening Service like Tiny URL 

URL Shortening Service:

Long URL ------> App ------> Short URL (6-8 characters)

Browser -----> Short URL ------> Long URL


Similar Services:
bit.ly, goo.gl, qlink.me, tinyurl



DEMO:
https://tinyurl.com/

Eg:
Long URL -----> Short URL


In Browser:
Short URL -----> Long URL



(1) Why do we need URL Shortening?

- Short URL: Much Easier to Use (Less typos/mistakes)
- Limit on character size: Short URLs are useful 
(Eg: Twitter: 140 chars)
- Affiliate Marketing: Tracking, Analysing Audience, and Campaign Performannces
- Hiding Sensitive Data


Tracking:
Link Shared: 1000
Viewed: 500
Clicked: 100





1 Short URL -----> More than 1 Long URL: Incorrect - Conflict Exception in Redirections

1 Long URL -----> More than 1 Short URL: Possible



(2) Requirements Clarification:

(A) Functional Requirements
- Given a Long URL, Users should be able to convert it into a short URL which is unique
- When that Short URL is Accessed, it should redirect to Long URL
- Each link will have a default timespan before it expires (5 Years)

(B) Non-Functional Requirements

- System should be HIGHLY Available
Because, If our System is Down, All URL Re-Directions will not Happen
- URL Redirections should happen in real-time with minimum latency

(C) Additional/Extended Requirements

- Users should be able to pick a custom URL
- Users can mention the expiry time of short URL
- Analytics: Redirections: Clicks, Views, CTR (Click Through Rate), Audience etc
- Third Party Integratins: REST API to Other Systems





(3) Capacity Estimation/ Back of the Envelope Estimation

Read-Heavy or Write-Heavy?


- Read-Heavy
- Lots of Re-Directions requests compared to new URL Shortenings
- R/W Ratio: 100: 1


(A) Traffic Estimates
(B) Queries Per Second
(C) Storage Estimates
(D) Cache Memory Estimates

Eg:

Read-Heavy Applications:
- More Number of Reads as Compared to Number of Writes in the System


Eg:
Blogging Website
Twitter
Youtube

Write-Heavy Applications:
- More Number of Writes as Compared to Number of Reads in the System


Eg:
Chat Communication (Whatsapp)




(A) Traffic Estimates:

Assumption,
500 Mn Write Requests in a Month


R/W Ratio: 100:1

No of Write Calls in a Month = 500 Mn
No of Read Calls in a Month = 500 Mn * 100 = 50 Bn


(B) Queries Per Second


------> Write Case:

New URL Shortenings = 500 Mn/Month
500 Million/ (30 Days * 24 Hours * 3600) ~= 200 URL/sec



------> Read Case:

R/W Ratio: 100:1

100 * 200 URL/sec = 20K URL Re-Directions/sec




(C) Storage Estimates


Assumption:
Every URL is stored for 5 Years/ Expires After 5 Years


Total Number of Links to Store:

500 Mn URLs every Month * 12 Months * 5 Years = 30 Billion URLs



https://tinyurl.com/abcdef12

Size for 8 chars: 16 Bytes

Additional Details:
user_uuid, location, creation_time, expiry_time, metadata etc

1 URL = 500 Bytes


Total Storage/Memory Required in 5 Years
= 30 Billion URLs * 500 Bytes
= 15 TB




(D) Cache Memory Estimates

20K Read Requests/second 8 3600 * 24 = 1.7 Billion Read Requests/Day

Following 80-20 Rule for My Cache:
Cache 20% of my data


To Store 20% of these Requests:
0.2 * 1.7 Billion * 500 bytes/URL = 170 GB of Cache Memory









(4) Defining the DB Schema

- Type of Data to Store
- Choice of DB
- Mapping Across different Tables/Databases



------> Observations:


(1) We need to store billions of records
(2) Each Object we store is small (~500 Bytes)
(3) There are NO Relationships between Records, 
apart from which User has created which URL
(4) Our Service is READ-HEAVY




-----> DB Schema:


Table-1: URL

- Hash(short_url): varchar: PK
- original_url: varchar
- creation_time: timestamp
- expiration_time: timestamp
- user_uuid: FK


Table-2: User

- user_uuid: Int: PK
- name: varchar
- email: varchar




------> Choice of Database?

We anticipate storing Billions of Records,
and We Dont Need to Use Relationships between Tables

Ideal DB:
NoSQL DB


Eg: Mongo DB, S3 etc








(5) Algorithms:

Major Part: How do you generate a short AND Unqiue URL?

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














