#!/usr/bin/env python3
import re
import datetime

class event:
    def __init__(self):
        self.date = None       # used in get_events_for_day to find any event on this datetime
        self.stime = None      # print formatted time
        self.etime = None      # print formatted time
        self.frequency = False # used in repetion method to determine whether the said event is to be repeated until rdate
        self.rdate = None      #the date by which the repeated event is to be ended
        self.location = None   #for printing
        self.summary = None    #for printing

class process_cal:
    #Constructor which take a string filename as a parameter
    def __init__(self, filename):
        self.filename = filename

    """
    get_events_for_day take a datetime value  as a parameter and return a string contain
    all events in readable and desired format
    """
    def get_events_for_day(self, datetime):
        list = self.fread()
        dlen = len(datetime.strftime("%B %d, %Y (%a)"))
        result = None
        """
        This loop determine if the list contain events took place on the given date as a parameter
        If so, variable result will be assigned to a formatted string of the datetime value along
        with the number of dashes based on the length of that string. If not, result will be equal
        to None by the end of the loop and will print nothing.
        """
        for x in list:
            if(x.date == datetime):
                result = datetime.strftime("%B %d, %Y (%a)") + "\n" + '-'*dlen
                break

        for x in list:
            if(x.date == datetime):
                result += "\n" + self.ftime(x.stime) + " to " + self.ftime(x.etime) + ": " + x.summary + " " + "{{" + x.location + "}}"
        return result;

    """
    fread read the list and collect values of DTSTART, DTEND, RRULE, LOCATION and SUMMARY using a for loop. An events object is created to
    collect these data for each event. When the loop reach a line containing "END:VEVENT", the newly created events object will be added
    to the end of the list and another events object is initialized for the next event. The function will return the list once the list is
    fully sorted.
    """
    def fread(self):
        list = []
        nevent = event();
        #This regex format will detect any date in the form of yyyymmddThhMMss with year ranging from 2000 - 2099
        dformat = r"(20\d{2})(\d{2})(\d{2})T(\d{2})(\d{2})(\d{2})"
        f = open(self.filename, "r")
        for x in f:
            if re.search("DTSTART:", x):
                a = re.search(dformat, x)
                nevent.date  = datetime.datetime(int(a.group(1)),int(a.group(2)), int(a.group(3)))
                nevent.stime = datetime.datetime(int(a.group(1)),int(a.group(2)), int(a.group(3)), int(a.group(4)), int(a.group(5)))
            elif re.search("DTEND:", x):
                b = re.search(dformat, x)
                nevent.etime = datetime.datetime(int(b.group(1)),int(b.group(2)), int(b.group(3)), int(b.group(4)), int(b.group(5)))
            elif re.search("RRULE:", x):
                c = re.search(dformat, x)
                nevent.frequency = True
                nevent.rdate = datetime.datetime(int(c.group(1)),int(c.group(2)), int(c.group(3)))
            elif re.search("LOCATION:", x):
                nevent.location = x[9:-1]
            elif re.search("SUMMARY:", x):
                nevent.summary = x[8:-1]
            elif re.search("END:VEVENT", x):
                list.append(nevent)
                nevent = event()
        self.repetion(list) #creating more event in another method if required
        list.sort(key=lambda x: (x.date, x.stime.time()))
        return list

    #repetion created more events in a given list if any event in that list required it.
    def repetion(self, list):
        for i in list:
            if(i.frequency == True):
                current_date = i.date
                while (current_date + datetime.timedelta(days=7)) <= i.rdate:
                    current_date += datetime.timedelta(days=7)
                    nevent = event()
                    nevent.date = current_date
                    nevent.stime = i.stime
                    nevent.etime = i.etime
                    nevent.frequency = False
                    nevent.rdate = None
                    nevent.location = i.location
                    nevent.summary = i.summary
                    list.append(nevent)

    """
    ftime take a string time format (e.g. 02:00 PM or 12:00 AM) and replace the first character
    of the string with a blank if it's equal to zero (e.g. 02:00 PM -> 2:00PM (with a blank in front))
    """
    def ftime(self, time):
        ntime = time.strftime("%I:%M %p")
        if(ntime[0] == '0'):
            ntime = ntime.replace("0"," ", 1)
        return ntime
