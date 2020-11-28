/*
From https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino

From https://github.com/TwilioDevEd/twilio_esp8266_arduino_example

Modified by Brian T. Park 2020 to compare the runtime of these routines to the
AceUtils UrlEncoding utilities which use the PrintStr utility classes instead
of String to avoid heap fragmentation. Added some tweaks to fix compiler
warnings about type cast problems and unused variables.
*/

#include "url_coding.hpp"

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}

String urldecode_yield(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < (int) str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{

        encodedString+=c;
      }

      yield();
    }

   return encodedString;
}

String urlencode_yield(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    //char code2;
    for (int i =0; i < (int) str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        //code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}

String urldecode_no_yield(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < (int) str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        encodedString+=c;
      }
    }

   return encodedString;
}

String urlencode_no_yield(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    //char code2;
    for (int i =0; i < (int) str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        //code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
    }
    return encodedString;
}
