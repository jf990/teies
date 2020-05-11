void conerrs(rc)
   int rc;
{
   int subnum = 0,
       errnum = 0;

   subnum = (rc % 1000) / 100;
   errnum = rc % 100;

   switch(subnum)
   {
   case CON_SET:
      s = ")"SET:";
      switch(errnum)
      {
      case 1:
         e = "Path opened to virtual dev(VD), but no real dev is connected";
         break;
      case 24:
         e = "The plist in invalid; path was not specified.";
         break;
      case 28:
         e = "Path is already open.";
         break;
      case 40:
         e = "The virtual dev is invalid or not defined.";
         break;
      case 88:
         e = "The virtual dev is not supported by CONSOLE for full-screen I/O";
         break;
      case 94:
         e = "Actually err 104. Unable to obtain storage to process request.";
         break;
      }  /* switch(errnum) */
      break;
   case CON_CLO:
      s = ")CLOSE:";
      switch(errnum)
      {
      case 3:
         e = "Requested path closed but other path still connected with device";
         break;
      case 24:
         e = "The plist in invalid; path was not specified.";
         break;
      case 28:
         e = "Path not found.";
         break;
      }
      break;
   case CON_READ:
      s = ")READ:";
      switch(errnum)
      {
      case 2:
         e = "MUST issue QUERY. Dev disconnected then reconnected.";
         break;
      case
