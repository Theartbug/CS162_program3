# CS162_program3

 gprovost@pdx.edu
  Program 3
  11/10/19
  CS 162

  Purpose: This program gives users the ability to create programming questions, store them externally, and load / display those questions at another time. They can also filter the questions by subject to display.

  Algorithm:
  1. Create constants to be used within the file:
     - int MAX_SUBJECT_SIZE
     - int MAX_DATE_SIZE
     - int MAX_QUESTION_SIZE
     - int MAX_NEW_QUESTIONS
     - int MAX_TOTAL_QUESTIONS
     - int NUMBER_OF_SUBJECTS
     - char[] FILE_NAME
     - char[] QUESTION_TOTAL_FILE
     - subjectArray[] subjects
  2. Create structs to use throughout the program:
     - subjectArray
     - question
     - structQuestionArray
     - totalQuestionArray
  3. Initalize variables within main
     - structQuestionArray newQuestions
     - totalQuestionArray loadedQuestions
     - bool questionsLoaded
     - bool newQuestionsSaved
     - bool questionsAdded
     - int menuChoice
     - int totalQuestions
  4. Create external files to hold data between program runs
     - 'questions.txt'
     - 'totalQuestions.txt'
         - holds the number of total quetions within questions.txt
  5. obtain the totalQuestion value with getTotalQuestionsNumber()
     - looks in "totalQuestions.txt" for the integer value
  6. wrap step 7 onwards in a do-while loop
     - the user will be able repeatedly go back to the main menu screen until they quit
  7. Ask the user what action they would like to preform, display those actions via getMainMenu(), and retrieve menuChoice. Use a switch case to determine what to do based on that menuChoice
  8. createQuetions() takes in newQuestions array and totalQuestions int ref
     - the user will be asked to enter question information with getQuestion()
     - the user will be asked with getResponse() if they would like to enter another question
  9. saveQuestions() takes newQuestions array and the total questions int
     - total questions is overwritten in 'totalQuestions.txt' so the value is correct the next time the program is started
     - questions are appended to the bottom of 'questions.txt'
  10. loadQuestions() takes the loadedQuestions array and places questions into it. Uses loadQuestion()
  11. displayAllQuestions() takes in loadedQuestions and displays all questions with displayQuestion()
  12. displayQuestionsBySubject() takes in loadedQuestions, gets user input with getSubject() and only displays based on a matched subject with displayQuestion()
  13. if the user selects 0 with menuChoice, they will exit the program
