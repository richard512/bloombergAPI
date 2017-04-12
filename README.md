# Python Script
The bbAPI.py is same with Excel BDH, BDP and BDS functions.

## Bloomberg Historical Data Request
df = BDH("AAPL US EQUITY", "PX_LAST", "20150101", "20150103")

## Bloomberg Data Point Request
df = BDP("AAPL US EQUITY", "PX_LAST")

## Bloomberg Data Set Request
df = BDS("AAPL US EQUITY", "DVD_HIST_ALL")

# BloombergAPI_cpp


# C++ Script
## Bloomberg Historical Data Request (BDH)
  without adjustment
  
      BDH("AAPL US EQUITY", "PX_LAST","20110101","20110104");
  with adjustment
  
      BDH("AAPL US EQUITY", "PX_LAST","20110101","20110104", true, true, true);

## Bloomberg Descriptive Data Request (BDS)
   without override
   
      BDS("AAPL US EQUITY", "DVD_HIST_ALL", false);
      
   with override
 
      BDS("AAPL US EQUITY", "DVD_HIST_ALL", true, "END_DATE_OVERRIDE","20170101");
      
## Bloomberg Single Data Request (BDP)
      
      BDP("AAPL US EQUITY", "PX_LAST");
