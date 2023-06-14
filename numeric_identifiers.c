/*********************************************
 * Id: cram1479
 *
 * Compile: gcc -Wall file.c
 * Run: ./a.out input.txt 
 *
 *  * file written in base c
 * Instructions:
 *  This assignment builds off the previous one. In this assignemnt you are 
 *   to expand your previous code to recognize the following in CCX.
 *  Numeric Literal
 *   CCX allows numeric literals in multiple forms. Your lexer will
 *   recognize a simplified subset of CCX numeric literals. Each numeric
 *   literal encountered by your lexer will start with a hexadecimal digit
 *   and will contain only the
 *   following:
 *    – hexadecimal digits (0 through 9, A through F and/or a through f)
 *    – the special character ’.’ (but it can only have one, so 1.2.3 is
 *       not a Numeric, but 1.2 is).
 *  Identifier
 *   Identifiers are used in programs to name entities such as variables.
 *   Every programming language has its own rules as to what constitutes a
 *   legal identifier. In CCX an identifier can be composed of letters,
 *   digits, and underscores, but must start with a letter. You may assume
 *   that your lexer will never encounter an identifier that is more than
 *   256 characters long.
 *  Hint: By now the only thing left unlexed are Tokens:
 *   Before you print a Token: see if it satisfies either of the constraints
 *   above. If it does, print it as a Numeric: or Identifier:
 *  Hint2: isxdigit, isdigit and isalpha might all be sueful to you.
 *********************************************/

// w08
//everything is indented by 3 spaces in c
//wdw = what am i dealing with
// 0 = token
// 1 = string
// 2 = char
// 3 = operator

#include <stdio.h> //include stand lib
#include <string.h> //include string lib
#include <stdlib.h> //need for free()
#include <ctype.h> //needed for isspace()

#define MAXTOKEN 256 //replaces MAXTOKEN with 256

//This funct breaks a line into tokens and strings
void tokeniszer( char *line, int length, int *totComments );

//This funct assigns meaning
void lex( char *line, int start, int end, int wdw );

//This funct strips white spaces off string+tokens passed in 
void WhiteSpaceDestroyer( char *item, int end, int wdw );

int main( int argc, char *argv [] ) //argc = # of input args
//                                     argv = pointer to input args
{
   if( argc < 2) //checks if valid input
   {
      printf( "You need to give the path name for input file " );
      printf( "%s /y/shared/Engineering/cs-drbc/submit/cs210/w03_readin1.txt             \n", argv[0] );
      return 1; //returns error
   }
   else
   {
      //printf("\nOpening file: %s \n", argv[1] ); wont submit if this here
   }

   FILE *fp; //declares file obj
   char *line = NULL; //char array pointer
   ssize_t read; //size of char array pointer
   size_t len = 0; 
   int totComments = 0; //counter var for total # of comments
   
   fp = fopen( argv[1], "r" ); //r specifies we read in from file
   //                             argv[1] specifies it as the 2nd file
   if( fp == NULL ) //if cant open file
   {
      printf( "Error: couldnt open file: %s\n", argv[1] );
      return 2; // returns error
   }

   while( ( read = getline( &line, &len, fp ) ) != -1 )// read = len of line
   {
      if( !( read == 1 && isspace( line[0] ) ) ) //filter for lines made of
      //                                          just 1 space
      {
	 //error checking: printf( "\n\nCurrent length is: %i\n", read );
         //                printf( "Current line: %s\n", line);
	 
	 tokeniszer( line, read, &totComments );
         memset( line, 0, read ); //frees contents of line
      }
   }
   
   fclose( fp ); //closes file
   
   if( line )
   {
     memset( line, 0, len ); //deallocated mem stored in line
   }
   
   return 0; //returns no errors
}

// this funct breaks a line into tokens and strings+chars
void tokeniszer( char *line, int length, int *totComments )
{
   int itemStart = 0; //when next item starts
   int totQuotes = 0; //keeps track of total # of quotes
   int totAposts = 0; //keeps track of total # of apostrophes
   int commentPholder = *totComments; //comment placeholder set
   int wdw = 0; //0 if token,1 if string, 2 if char, 3 if operator
   char quote = '"';
   char chr = '\''; //syntax for setting a char to aposte
   char fSlash = '\\'; //used reading in an fSlash to compiler
   char bSlash = '/';
   char star = '*';
   int i; //for loop counting var
   int j; //comment for loop counting var
   //int k; //operator for loop counting var
   char operators[MAXTOKEN] = {" < > ( ) + - * / | & ; , : = $ @ := .. << >> <> <= >= ** != => [ ] { } {: }: "};
   char findOps1[5];
   char findOps2[4];
   int yesOp;
   int justEndedComment = 0;
   //int k; //char for loop counting var
   //int charSpace = -1; //specifies spot where space is after an apost found
   //int charAfterSpace = -1;

   for( i = 0; i < length; i++ ) //counts thru entire line
   {
     //printf("line: %s %c %d\n", line, line[i],i); 

      if( totQuotes % 2 == 0 ) //if not curr tween quotes
      {

	 // comment section:
         if(commentPholder % 2 == 0 && line[i] == bSlash &&                          line[i+1] == star )
	 // if not in comment+finds comment start star bSlash 
	 {
	    commentPholder++; //only adds if bSlash star not in comment
	    *totComments = commentPholder;
	 }
     
	 if( commentPholder % 2 == 1 ) //if inna comment
	 {

	    printf("Comment: ");
	
	    for( j = i; j < length; j++ )
	    {
	       printf("%c", line[j]); 
	       //        prints every char in comment besides last backslash 
	   
	       if( line[j+1] == bSlash && line[j] == star )
	          // if finds end comment
	       {
		  printf( "%c\n", line[j+1] ); // prints last bSlash char
		  commentPholder++;//adds 1 to tot if comment terminated
		  itemStart = j + 2; //skip star bSlash
		  i = j + 1; //picks up at last bSlash
		             // (incase back to back comments)
		  justEndedComment = 1;
		  break; // break out of curr for loop
		 
	       }
	    }

	    *totComments = commentPholder;

	    if( j == length )
	       //if prints out whole line bc multi line comment
	    {
	       break; // break out of 1st for loop to stop reading line
	    }
  
	 }
      }
      if( commentPholder % 2 == 0 ) //if not in comments
      {
	 if( totQuotes % 2 == 0 ) // if not in quotes
	 {
	   
           if( (totAposts % 2 == 0) ) //||(totAposts % 2 == 1 && 
	     //	(isspace(line[i])))) // included 
                    //if not tween aposts or a space is tween aposts   
	     {
	       
	       //OPERATOR SECTION:
	       //initializes findOps2
	       findOps2[0] = ' ';
	       findOps2[1] = line[i];
	       findOps2[2] = line[i+1];
	       findOps2[3] = ' ';  //word to search for
	       findOps2[4] = '\0';
	       //initializes findOps1
	       findOps1[0] = ' ';
	       findOps1[1] = line[i];
	       findOps1[2] = ' '; //char to search for
	       findOps1[3] = '\0';

	       //search for 2 char op first
	       //printf("findOps:%s\n", findOps ); 
	       while( strstr(operators,findOps2) != NULL ||                                strstr(operators,findOps1) != NULL )//an op found
	       {
		   while( strstr(operators,findOps2) != NULL )
		   {
		     //printf("finding: %s\n", findOps);
		      wdw = 3;
		      itemStart = i;
		      lex( line, itemStart, i+2, wdw ); 
		      itemStart = i+2;
		      i = i+2; //jmp i so dont double readin same chars
		      findOps2[1] = line[i];
		      findOps2[2] = line[i+1]; //redefs chars to find
		   }
		   
		   findOps1[1] = line[i];

		   //search for 1 char op now
		   while( strstr(operators,findOps1) != NULL )
		   {
		     
		      if( justEndedComment == 1 )
		      {
		         break; //dont accidently readin end of comment
		      }
		     
		      findOps2[1] = line[i];
		      findOps2[2] = line[i+1]; 
                        //updates chars to find in case this op is 2 chars
		      
		      if( strstr(operators,findOps2) != NULL )
		      {
			 break; //checks if 2 char operator
		      }
		      //findOps[1] = line[i]; //redefs char to find
		      wdw = 3;
		      itemStart = i;
		      lex( line, itemStart, i+1, wdw ); //pass char as op
		      itemStart = i+1;
		      i = i+1;
		      findOps1[1] = line[i]; //redefs char to find
		   }

		   if( justEndedComment == 1 )
                   {
		      break; //dont accidently readin end of comment
		      // need 2 breaks for 2 while loops
		   }

		   findOps2[1] = line[i];
		   findOps2[2] = line[i+1]; //updates chars to find
		   
		   findOps1[1] = line[i];
		    
	       }                 //loops again if next char a 2 char operand

		   findOps2[1] = line[i+1];
		   findOps2[2] = line[i+2];

		   findOps1[1] = line[i+1]; //updated to most recent chars

	       if( (strstr(operators,findOps2) != NULL) || 
		   (strstr(operators,findOps1) != NULL) )
	       {
		 yesOp = 1; //next char is an operator
	       }

	       memset( findOps1, 0, 4 ); //free mem
	       memset( findOps2, 0, 5 ); //free mem

	       // token section:
	       if( (isspace(line[i])) ||
		   (line[i] == quote && line[i-1] != chr ) ||
		   (line[i] == chr && line[i-1] != quote ) ||
		   (yesOp == 1) ) //line[i] != quote && 
		     //line[i] != chr) )// (tacked onto end of yesOp?
		   
		 //checks if char=white space, or if curr char is starting
		 // quote(to pass in token prev to string)
		 // should pass in if char before quote isnt a chr??
		 // has line[i-1] == isspace(line[i]) ) ) before
		 {
		   //check: printf("Got here: %d %d\n", itemStart, i);
		   wdw = 0;
		   if( yesOp == 1 ) //if next char an op
		   { 
		      lex( line, itemStart, i+1, wdw);
		      itemStart = i+1;
		      yesOp = 0; //reset yesOp
		   }
		   else{
		      lex( line, itemStart, i, wdw);
		      //    passes curr word coordinates+whole line to lex
		      itemStart = i+1; //placeholder put after end of word
		      //              to lex to signify new start point
		   }
		 }
	     }

	   
	   //char section:
	   if( (line[i] == chr) )
	   {
	      if( totAposts % 2 == 1 ) //runs if 2nd apost in pair found
	      {
		
	         if( line[i-1] == fSlash && line[i-2] != fSlash ) 
                 // if fSlash before apost+not a fSlash before it
                 // (covers fSlash declaration)         
		 {
		    totAposts --; //dont pass and ignore apost
		 }
	       
		else{
		    wdw = 2; //passes in as a char
		    lex( line, itemStart, i+1, wdw);
		    itemStart = i+1; 
		    //     skip over 2nd apost so doesnt print it twice
		}
	      }
	      else{ //1st apost found
	
		 itemStart = i;
		
	      }
	      totAposts++; //adds 1 for an apost discovered
	    }
	 }
	   
	 // quote section:
	if( totAposts % 2 == 0 ) //if not tween aposts
	{ 
	 if( line[i] == quote ) //runs if curr char a quote
	 {
	    if( totQuotes % 2 == 1 ) //runs if 2nd quote in pair found
	    {
	       wdw = 1; //passes in as a string
	       lex( line, itemStart, i+1, wdw); 
	       //add 1 to i bc funct not used to reading in 2nd aposte 
	       // or quote
	       itemStart = i+1; //add 1 so doesnt double read in quote 
	    }
	    else{
	       itemStart = i;
	    }
	    totQuotes++; //adds 1 for a quote discovered
	 }
	}
      }
      justEndedComment = 0;
   } 
}

//This funct assigns meaning
void lex( char *line, int start, int end, int wdw )
{
   char item[MAXTOKEN]; //new item
   int whiteSpaceCount = 0; //# of white spaces in new item
   int newItemLen = end-start; //length of new item
   int i; //for loop counting var

   //error checking:    printf("Start index: %i\n", start);

   // printf("End index: %i\n", end);
  
   for( i = 0; i < newItemLen; i++ ) //count # of white spaces in new word
   {
      if( isspace( line[start+i] ) )
      {
         whiteSpaceCount++;
      }
   } 

   if(  whiteSpaceCount != newItemLen )
   {

      if( wdw == 1 ) //if string
         //                                     +whole line not white space
      {
         strncpy( item, &line[start], newItemLen ); 
	 //                                    copies line segment to item
	 
	 item[newItemLen] = '\0'; //terminates string
	
	 printf("String: %s\n", item);
	 //dont need:	WhiteSpaceDestroyer( item, end, wdw ); //pass
	
      }
      
      if( wdw == 2 ) //if char
	//                                     +whole line not white space
	{
	  strncpy( item, &line[start], newItemLen );
	  //                                    copies line segment to item

	  item[newItemLen] = '\0'; 
	   //terminates string early bc when create char, 
           // cut off 1 char late 

	  printf("Char: %s\n", item);
	  //dont need:  WhiteSpaceDestroyer( item, end, wdw ); //pass

	}
      
      if( wdw == 0 ) //|| wdw == 2 ) //if char or token 
	 //                                    +whole line not white spaces
      {
	 strncpy( item, &line[start], newItemLen ); //copies passed in word
	 //                                             to item 
	 item[newItemLen] = '\0';  //'\0' = null termination char
	 //                            specifies last char as end of word
	 //printf("Item: %s\n", item);
	 //checking: printf("Length of below word is: %i\n", newItemLen);
	 //             printf("Token: %s\n", item);
	 //checker for post word spaces: printf("Waffle\n");
	 if( strlen( item ) == 1 && item[0] == '\'' )
	   // to get rid of one apost token after operator found
	 {

	 }
	 else{
	    WhiteSpaceDestroyer( item, end, wdw );
	 }
      }
      if( wdw == 3 )
      {
	strncpy( item, &line[start], newItemLen ); //copies passed in word
	//                                             to item
	item[newItemLen] = '\0';  //'\0' = null termination char
	//                            specifies last char as end of word
	printf("Operator: %s\n", item);

      }
   }
}

//This funct strips white spaces off strings+tokens passed in
void WhiteSpaceDestroyer( char *item, int end, int wdw )
{
   char correctItem[MAXTOKEN]; //new array
   int w = 0; //new array char loc
   int i; //for loop counting var
   char keywordCheck[MAXTOKEN] = " accessor and array bool case character constant else elsif end exit float func if ifc in integer is mutator natural null of or others out pkg positive proc ptr range subtype then type when while ";
   char keywordPH[MAXTOKEN];
   int totPeriods = 0;
   int j = 0; //identifier for loop counting var

   keywordPH[0] = ' ';//wireframe to create word to search for



   //printf("end is for this word: %i\n", end);

   for( i = 0; i < end; i++ ) //loops thru whole word/string
   {
     if( !( isspace( item[i] ) ) && wdw != 1) 
      //             if not char or token+ not whitespace
      {
         correctItem[w] = item[i]; //give char to another array
	 //printf("correctItem: %c\n", correctItem[w] );
         w++; //increment new array
	 
      }
   }

   if( wdw == 0 ) //if token
   {
     keywordPH[strlen(correctItem)+1] = ' ';
     keywordPH[strlen(correctItem)+2] = '\0';// more wireframe
     //bc keyword runs thru as token up until here

     for( i = 0; i < strlen(correctItem); i++ ) 
       //loops thru whole word/string
     {
       keywordPH[i+1] = correctItem[i]; //copy array into wireframe
     }
     //check: printf("Searching list for: %s%s\n", keywordPH,"end");
     //    printf("Last letter in keyword search is %c\n", keywordPH[end]);
     if( strstr( keywordCheck,keywordPH ) != NULL ) 
     //checks if token=keyword by seeing if can find token wireframe in a
     // string of all possible keywords 
     {
        printf("Keyword: %s\n", correctItem);   //if keyword
     }
     else{

       for( i = 0; i < strlen(correctItem); i++ ) 
                       //loops thru whole word/string
       {
	 if( isxdigit( correctItem[i] ) || correctItem[i] == '.'  ) 
         //if all digits hex or '.'
	 {
	   if( correctItem[i] == '.' )
	   {
	     totPeriods++;
	     if( totPeriods > 1 )
	       {
		 break; //break if not numberic bc more than one period
	       }
	   }  
	 }
	 else{
	   break; //break if not a numeric
	 }
       }
       if( i == strlen( correctItem ) ) 
       //if didn't break from for loop, it's a numeric
       {
	 printf("Numeric: %s\n", correctItem);
       }
       else{

	 if( isalpha( correctItem[0] ) ) //if first char = letter
	 {
	    for( j = 0; j < strlen(correctItem); j++ )
            {
	      if( isdigit( correctItem[j] ) || isalpha( correctItem[j] )||
                  correctItem[j] == '_' )
		// if all letters,digits, and '_'
	      {

	      }
	      else{
		break;
	      }
	    }
	 }
	 if( j == strlen( correctItem ) )
	   //if didn't break from for loop, it's an identifier
	   {
	      printf("Identifier: %s\n", correctItem);
	   }
	 else{
	    printf("Token: %s\n", correctItem);
	 }
       }
     }
   } 

   if( wdw == 2 ) //if char
   {
      printf("Char: %s\n", correctItem);
   }

   if( wdw == 1 ) //if string
   {
      printf("String: %s\n", correctItem);
   }

   /*
   if( wdw == 3 ) //if char
   {
      printf("Operator: %s\n", correctItem);
   }
   */

   memset( keywordPH, 0, MAXTOKEN ); //frees contents of correctItem
   memset( correctItem, 0, MAXTOKEN ); //frees contents of correctItem
  
}
