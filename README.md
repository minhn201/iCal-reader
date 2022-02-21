# iCal-reader
-----

**Summary**
This program, written in Python, will input file in ics format to output a readable text of the given file. The program can be executed using a bash command in a Linux environment.

**What is an ics file?**
An ics file is a plain text file that included calendar event details such as a description, start and end times, location, and so on. An example of an event in ics file would be,

```
BEGIN:VCALENDAR
BEGIN:VEVENT
DTSTART:20210214T180000
DTEND:20210214T210000
LOCATION:Campus 
SUMMARY:Delevoping a software with friends.
END:VEVENT
END:VCALENDAR
```

From above, we can deduce that the event is expected to happen on February 14, 2021 from 6:00 PM to 9:00 PM. The date and time are indicate in ```yyyymmddThhmmss``` format. It will be about "Developing a software with a friend" and will take place on university campus. An ics file may contain mulitple event instants, and each event can indicate that it will be repeated until a certain time. Considering the following example,

```
BEGIN:VCALENDAR
VERSION:A
BEGIN:VEVENT
DTSTART:20210101T111500
DTEND:20210101T123000
RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=20210301T235959;BYDAY=SA
LOCATION:Starbuck
SUMMARY:Coffee with Patrick
END:VEVENT
END:VCALENDAR
```

This event is expected to be repeated weekly from Jan 1, 2021 to March 1, 2021 as indicate in ```RRULE```. Every repeated instance will take place in the same locoation within the given time range (e.g. in the above example, it will be 11:15 AM to 12:30 PM)
