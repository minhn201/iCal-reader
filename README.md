# iCal-reader

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

From above, we can deduce that the event is expected to happen on February 14, 2021 from 6:00 PM to 9:00 PM. The date and time are indicate in ```yyyymmddThhmmss``` format. It will be about "Developing a software with a friend" and will take place on university campus. An ics file may contain mulitple event instants, and each event can also indicate that it will be repeated until a certain time. Considering the following example,

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

This event is expected to be repeated weekly from Jan 1, 2021 to March 1, 2021 as indicate in ```RRULE```. Every repeated instance will take place in the same locoation within the given time range (e.g. in the above example, it will be 11:15 AM to 12:30 PM).

**How does the program work?**

Since ics files are very difficult to interpret if we are to know many events inside them, a program is needed to produce a readable version when a file is inputted.

With a given file, the ical reader will parse it to collect data represent an event, which will then be store into a object. There will be a collection of objects that constitute all events. The program may also create more same instance of event object if needed (e.g. if the event is to be repeated). 

After data gathering, the list of objects will be sorted according first according date and time. The program will then printed the events when called using bash command. Only events that are within the date range indicated in the command will be printed.

**Examples**

With an sample file,

```
BEGIN:VCALENDAR
BEGIN:VEVENT
DTSTART:20210214T180000
DTEND:20210214T210000
LOCATION:Burger King
SUMMARY:Romantic dinner with Chris
END:VEVENT
END:VCALENDAR
```

We can expect the program to produce the following,

```
February 14, 2021 (Sun)
-----------------------
 6:00 PM to  9:00 PM: Romantic dinner with Chris {{Burger King}}
```

If an event is expected to repeated until a given time, like this example,

```
BEGIN:VCALENDAR
VERSION:A
BEGIN:VEVENT
DTSTART:20210206T111500
DTEND:20210206T123000
RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=20210301T235959;BYDAY=SA
LOCATION:The Bumptious Barista
SUMMARY:Coffee with Pat
END:VEVENT
END:VCALENDAR
```

The output should be,

```
February 06, 2021 (Sat)
-----------------------
11:15 AM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}

February 13, 2021 (Sat)
-----------------------
11:15 AM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}

February 20, 2021 (Sat)
-----------------------
11:15 AM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}

February 27, 2021 (Sat)
-----------------------
11:15 AM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}

```

We can also use bash command to print events in a given range if we don't want to read all the events in a single file.
