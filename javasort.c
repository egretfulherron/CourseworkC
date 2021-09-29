/* This program deals with .java files by reading them in and printing part of the 
document to a seperate file, and other parts to the terminal/console.
Author- Emily Herron.
*/
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
        //open files
    FILE *input_file; 
    FILE *output_file; 
	input_file = fopen(argv[1], "r"); 
	output_file = fopen(argv[2], "w"); 

    //variables 
    int line_count= 0; 
    int java_count = 0;
    int flag_param = 0;
    int flag_return = 0;
    int flag_author = 0;
    int method_flag = 0;
    int i;
    int tag_flag = 0; // for finding the next word after @ tag
    int name_flag = 0;
    int class_flag = 0;
    int printing_flag = 1;
    int param_counter = 0;
    char sentence[1000];
    char second_value[1000];
    char *last_word; // for storing the method name
    char tag_value[1000];
    int non_blank_flag = 0;
    int non_blank_counter = 0;
    int this_one;
   // int l = 0; <- this was the argv searcher which I couldn't get to work
        // while(l<argc){ 
        //     if (strcmp(argv[l], "-i")){
        //         this_one = l+1;
        //     }
        //     else if (strcmp(argv[l], "-o")){
        //     that_one = l+1;
        //     }
        //     l++;
        // }
        while (fgets(sentence, 1000, input_file) != NULL) { //read in file

            int string_length = strlen(sentence); 
            int sentence_index = 0;
            line_count++; //for total line count
            
                for (int i = 0; sentence[i]; i++) { //for each char
                    char a = sentence[i];
                           if(printing_flag == 1){  // this is on at the start of each javadoc comment
                                if (sentence[i]!= '{'){ //this will stop printing after the {
                                    if(sentence[i] != '}'){ // these kept printing and this prevented it
                                        fprintf(output_file, "%c", sentence[i]); 
                                    }
                                }
                                else if (sentence[i] == '{'){  // reached the end of the useful part, before the body of the method
                                    fprintf(output_file, "%c", sentence[i]);
                                    printing_flag = 0;
                                }
                            }   
                            
                            if(sentence[i] == '}'){ //turn printin back on when we are about to hit the start of the javadoc comment
                                printing_flag = 1;
                                java_count++;
                            }    
                            
                            if (a != '\0' && a != '\t' && a!=' ' && a!= '\n'){ //checks each char for non blank
                                non_blank_flag=1;
                            }
                }
 
			    if (non_blank_flag == 1){ //if throughout the whole line, there is one non space char 
				    non_blank_counter ++; 
					non_blank_flag = 0; //reset before next line
				}

			char *delimiters = "\t \n"; // Symbols that mark the end of a word/token
            char *token = strtok(sentence, delimiters); // load the first word/token

                while (token != NULL) { //while there are tokens to process
                    if(name_flag == 1){ //this is activated when we hit 'public' 
                        if (strcmp(token, "(") == 0 || strcmp(token, "{") == 0) { //when it reaches the end of the name	...			
                            name_flag = 0; 
                            //name_counter = 0;
                    
                            if (method_flag ==1){ //activated if param/return tag has been found
                                printf("Method %s \n",last_word); //last_word is found below, but it is the name of the method/class
                                    if(param_counter == 1){ //++ when @param tag is found
                                        printf("Parameters : %s\n",tag_value); ///tag value holds the words following @ tag
                                        param_counter = 0;
                                        tag_flag = 0;
                                        memset(tag_value, 0, sizeof(tag_value)); //tag_value reset to 0 for next method/class
                                        flag_param = 0;
                                        method_flag = 0;
                                    }
 					            	else if(param_counter == 2){
                                    printf("Parameters : %s\n",tag_value);
                                	printf("Parameters : %s\n",second_value);
                                    param_counter = 0;
                                    tag_flag = 0;
                                    memset(tag_value, 0, sizeof(tag_value));
                                    flag_param = 0;
                                    method_flag = 0;
                                }

                                    if (flag_return == 1){
                                        printf("Returns : %s\n",tag_value);  //same as above but for @return   
                                        flag_return = 0;  
                                        tag_flag = 0;
                                        memset(tag_value, 0, sizeof(tag_value));
                                        method_flag = 0;
                                    }                 
                            }               
                            else if(class_flag == 1){ // same logic as method_flag , but prints Class and Author in place of method +param/return
                                printf("Class %s \n",last_word);
                                name_flag = 0;
                                class_flag = 0;
                                printf("Author : %s\n",tag_value);
                                flag_author = 0;
                                tag_flag = 0;
                                memset(tag_value, 0, sizeof(tag_value));
                            }
                   
                        } else {
                            last_word = token; //last_word is the final word before the ( or {. this loops until it finds either, then prints last_word
                        }
                    }
                    
                    if(tag_flag == 1){
                        if (strcmp(token, "*/") == 0){//strcmp(token, "*") == 0) { //these mark the end of the 
                            tag_flag = 0; // sets to say we have passed the useful part
                            printf("\n");
                        } 
                        // if(strcmp(token, "*") == 0) {
                        //     star_flag = 1;
                        // }
                 
                        else if(param_counter == 2){
                            strcat(second_value, token); //if this is the second @param in one method, then we add it to a new variable
                            strcat(second_value, " ");

                        } else {
                            strcat(tag_value, token); //if we haven't reached the end, add the word and a space to variable
                            strcat(tag_value, " ");
                        }
                    }

                    if (strcmp(token, "@param") == 0) { //if it contains... set flags which have been explained for the next iteration (same for all)
                        flag_param = 1;
                        param_counter++;
                        tag_flag = 1;
                        method_flag = 1;

                    } else if (strcmp(token, "@return") == 0) {   
                        flag_return = 1;
                        tag_flag = 1;
                        method_flag = 1;

                    } else if (strcmp(token, "@author") == 0) {
                        flag_author = 1;
                        class_flag = 1;
                        tag_flag = 1;
        

                    } else if (strcmp(token, "public") == 0) {
                        name_flag = 1;

                    }
                    token = strtok(NULL, delimiters); //load next token
                }
            }
    printf("\nTotal number of lines: %d\n", line_count); 
    printf("Number of non-blank lines: %d\n", non_blank_counter);
    printf("Number of Javadoc comments: %d\n", java_count);
    fclose(input_file); //close files
    fclose(output_file);
    return 0;
}