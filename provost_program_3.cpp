/*  Grace Provost
  *  gprovost@pdx.edu
  *  Program 3
  *  11/10/19
  *  CS 162
  *
  *  Purpose: This program gives users the ability to create programming questions, store them externally, and load / display those questions at another time. They can also filter the questions by subject to display.
  *
  *  Algorithm: 
  *  1. Create constants to be used within the file:
  *     - int MAX_SUBJECT_SIZE
  *     - int MAX_DATE_SIZE
  *     - int MAX_QUESTION_SIZE
  *     - int MAX_NEW_QUESTIONS
  *     - int MAX_TOTAL_QUESTIONS
  *     - int NUMBER_OF_SUBJECTS
  *     - char[] FILE_NAME
  *     - char[] QUESTION_TOTAL_FILE
  *     - subjectArray[] subjects
  *  2. Create structs to use throughout the program:
  *     - subjectArray
  *     - question
  *     - structQuestionArray
  *     - totalQuestionArray
  *  3. Initalize variables within main
  *     - structQuestionArray newQuestions
  *     - totalQuestionArray loadedQuestions
  *     - bool questionsLoaded
  *     - bool newQuestionsSaved
  *     - bool questionsAdded
  *     - int menuChoice
  *     - int totalQuestions
  *  4. Create external files to hold data between program runs
  *     - 'questions.txt'
  *     - 'totalQuestions.txt'
  *         - holds the number of total quetions within questions.txt
  *  5. obtain the totalQuestion value with getTotalQuestionsNumber()
  *     - looks in "totalQuestions.txt" for the integer value
  *  6. wrap step 7 onwards in a do-while loop
  *     - the user will be able repeatedly go back to the main menu screen until they quit
  *  7. Ask the user what action they would like to preform, display those actions via getMainMenu(), and retrieve menuChoice. Use a switch case to determine what to do based on that menuChoice
  *  8. createQuetions() takes in newQuestions array and totalQuestions int ref
  *     - the user will be asked to enter question information with getQuestion()
  *     - the user will be asked with getResponse() if they would like to enter another question
  *  9. saveQuestions() takes newQuestions array and the total questions int
  *     - total questions is overwritten in 'totalQuestions.txt' so the value is correct the next time the program is started
  *     - questions are appended to the bottom of 'questions.txt'
  *  10. loadQuestions() takes the loadedQuestions array and places questions into it. Uses loadQuestion()
  *  11. displayAllQuestions() takes in loadedQuestions and displays all questions with displayQuestion()
  *  12. displayQuestionsBySubject() takes in loadedQuestions, gets user input with getSubject() and only displays based on a matched subject with displayQuestion()
  *  13. if the user selects 0 with menuChoice, they will exit the program
*/

#include <iostream>
#include <cctype>
#include <cstring>
#include <map>
#include <fstream>

using namespace std;

// CONSTANTS AND GLOBALS   

// the max character size for a question subject
const int MAX_SUBJECT_SIZE = 15;
// max date size xx/xx/xxxx and includes space for '\n'
const int MAX_DATE_SIZE = 11;
// max character size for question
const int MAX_QUESTION_SIZE = 130;
// max size for new questions array
const int MAX_NEW_QUESTIONS = 10;
// maz size for total questions that can be
// loaded from external file
const int MAX_TOTAL_QUESTIONS = 100;
// the current number of subjects
// must be increased if any more are added
const int NUMBER_OF_SUBJECTS = 6;
// to store the file name we use
const char FILE_NAME[13] = "question.txt";
// to store the file name for the total number of questions
const char QUESTION_TOTAL_FILE[19] = "totalQuestions.txt";
// a struct to pair size with the question subject
struct subjectArray {
    char name[MAX_SUBJECT_SIZE];
    int size;
};
// an array to contain subjects so any function can access
// new subjects can be created here
// order does matter
const subjectArray subjects[NUMBER_OF_SUBJECTS]  = {
    {
        "condition flow",
        14
    },
    {
        "loops",
        5
    },
    {
        "arrays",
        6
    },
    {
        "functions",
        9
    },
    {
        "structures",
        10
    },
    {
        "external files",
        14
    },
};
// a struct to pair all the question elements
struct question {
    int number;
    char date[MAX_DATE_SIZE];
    int subject;
    char text[MAX_QUESTION_SIZE];
};
// a struct to keep the 10 questions and size paired together
struct structQuestionArray {
    int size;
    question array[MAX_NEW_QUESTIONS];
};
// a struct to keep the 100 max loaded questions together
struct totalQuestionArray {
    int size;
    question array[MAX_TOTAL_QUESTIONS];
};

// FUNCTION PROTOTYPES

// displays the main menu and obtains the users
// response to what they want to do
int getMainMenu(void);

// a helper function for cleaning out the buffer
// since we will be doing it multiple times
void clearBufferStream(void);

// this helper obtains characters and places them into an array
// via array reference
// placement happens insde the while loop expression
// since arrays use references, nothing needs to be returned
// while also validating proper length of input
// it takes an array reference to place the string
// as well as the array size desired
int getString(char array[], int arraySize);

// this helper obtains a single character
// checks if it is a valid y / n response
// and returns a boolean based on if the response
// is yes = true, no = false
bool getResponse(void);

// obtains an integer response
// between a certain inclusive range
int getIntegerRangeResponse(int max, int min);

// obtains user input to construct questions
// and add to given newQuestions array
// returns a boolean
void createQuestions(structQuestionArray& newQuestions, int totalQuestions);

// takes in a question struct and promps the user to enter in 
// responses for the question contents
void getQuestion(question& question);

// displays given question in terminal
void displayQuestion(question& question);

// displays all loaded questions
void displayAllQuestions(totalQuestionArray& newQuestions);

// saves questions entered to external file
void saveQuestions(structQuestionArray& newQuestions, int totalQuestions);

// load questions from external file
// takes in a struct to place the loaded questions in
void loadQuestions(totalQuestionArray& loadedQuestions);

// loads a single question from a passed ifstream
void loadQuestion(ifstream& in, question& question);

// displays questions by subject
// takes in the loadedQuestions struct
void deisplayQuestionsBySubject(totalQuestionArray& loadedQuestions);

// get subject
int getSubject(void);

// displays questions by subject
// takes in the loadedQuestions struct
void displayQuestionsBySubject(totalQuestionArray& questions);

// get total number of questions
// takes in a reference to totalQuestions
void getTotalQuestionsNumber(int& totalQuestions);

// switch case to interpret users selection
void menuSwitch(int menuChoice, int& totalQuestions,  structQuestionArray& newQuestions, totalQuestionArray& loadedQuestions, bool& questionsLoaded, bool& newQuestionsSaved, bool& questionsAdded);

int main(void)
{
    // VARIABLES

    // to hold the 10 new possible questions
    structQuestionArray newQuestions;
    // to hold the loaded questions from an external file
    totalQuestionArray loadedQuestions;
    // to keep track if questions have been loaded
    bool questionsLoaded = false;
    // to keep track if new questions have been saved
    bool newQuestionsSaved = false;
    // to keep track if new questions have been added   
    bool questionsAdded = false; 
    // to keep track of what option the user choses from the 
    // main menu. If zero, will end program
    int menuChoice = 0;
    // total number of questions in external file
    int totalQuestions = 0;
    // get the total number of questions stored externally
    // so we will be able to give each question a correct number
    getTotalQuestionsNumber(totalQuestions);
    // welcome the user and show main menu to begin
    cout << "Welcome. This program collects and saves programming practice questions. Please select what you would like to do:" << endl;
    do
    {
        menuChoice = getMainMenu();
        cout << "________________________________" << endl;
        // switch case to interpret users selection
        menuSwitch( menuChoice, totalQuestions, newQuestions, loadedQuestions, questionsLoaded,  newQuestionsSaved, questionsAdded);
    
    } 
    while(menuChoice != 0);

    cout << "Goodbye!" << endl;
    return 0;
}

// FUNCTION DECLARATIONS

// switch case to interpret users selection
void menuSwitch(int menuChoice, int& totalQuestions,  structQuestionArray& newQuestions, totalQuestionArray& loadedQuestions, bool& questionsLoaded, bool& newQuestionsSaved, bool& questionsAdded)
{
    switch(menuChoice)
    {
        case 1:
            // dont let the user add more questions if they already have
            if (!questionsAdded) 
            {
                cout << "Let's create some new pratice questions!" << endl;
                createQuestions(newQuestions, totalQuestions);
                questionsAdded = true;
            }
            else cout << "You've already entered questions. Quit the program to enter more again." << endl;
            break;
       case 2:
            // dont let the user save if they havent added questions
            if (questionsAdded && !newQuestionsSaved)
            {
                saveQuestions(newQuestions, totalQuestions);
                newQuestionsSaved = true;
            }
            // dont let the user re-save if they already have
            else if (questionsAdded && newQuestionsSaved) cout << "You've already saved your added questions." << endl;
            // user must enter questions to save
            else cout << "You must first enter some questions in order to save." << endl; 
            break;
        case 3:
            // if we've not loaded before, let them
            if(!questionsLoaded) 
            {
                loadQuestions(loadedQuestions);
                questionsLoaded = true;
            }
            // no need to reload
            else cout << "You've already loaded questions!" << endl;
            break;
       case 4:
            // if the user has not loaded, dont print
            if(questionsLoaded) displayAllQuestions(loadedQuestions);
            else cout << "You need to load questions before displaying them!" << endl;
            break;
       case 5:
            // if the user has not loaded, dont print
            if(questionsLoaded) displayQuestionsBySubject(loadedQuestions);
            else cout << "You need to load questions before displaying them!" << endl;
            break;
        default: 
            break;
    }
}



// get total number of questions
// takes in a reference to totalQuestions
void getTotalQuestionsNumber(int& totalQuestions)
{
    // get the fstream involved
    ifstream in;
    // open the file
    in.open(QUESTION_TOTAL_FILE);
    if(in) in >> totalQuestions;
    // close out the file
    in.close();
    // clear the connection
    in.clear(); 
    return;
}



// get subject
int getSubject(void)
{
    cout << "Subjects:" << endl;
    cout << "1. Condition Flow" << endl;
    cout << "2. Loops" << endl;
    cout << "3. Arrays" << endl;
    cout << "4. Functions" << endl;
    cout << "5. Structures" << endl;
    cout << "6. External Files" << endl;
    cout << "Select a subject for your question (1-6): ";
    // obtain the response offset by one to match the subject array
    return getIntegerRangeResponse(6,1) - 1;
}



// displays questions by subject
// takes in the loadedQuestions struct
void displayQuestionsBySubject(totalQuestionArray& questions)
{
    // because we have to
    int i;
    // keep track of what is actuallyed displayed
    int questionsDisplayed = 0;
    // easier to read in function
    question currQuestion;
    // subject selected by user to display
    int subject;
    
    cout << "Select a subject you would like to see from loaded questions" << endl;
    subject = getSubject();
    cout << "Showing loaded questions of the subject " << subjects[subject].name << ": " << endl << endl;
    // loop over questions and only show those that are the selected subject
    for(i = 0; i < questions.size; ++i)
    {
        currQuestion = questions.array[i];
        if (currQuestion.subject == subject)
        {
        displayQuestion(currQuestion);
        questionsDisplayed++;
        }
    }
    // tell the user how many questions were displayed
    if(questionsDisplayed > 0) cout << questionsDisplayed << " question(s) displayed!" << endl;
    // or if none were found of that type
    else cout << "No questions exist of that subject!" << endl;
    return;
}



// load questions from external file
// takes in a struct to place the loaded questions in
void loadQuestions(totalQuestionArray& loadedQuestions)
{
    // get the fstream involved
    ifstream in;
    // because we have to
    int i;
    // open the file
    in.open(FILE_NAME);
    if(in)
    {
        // while we are still in a valid state and have no reached the end of the file
        // loop with incrementing i
        for(i = 0; in && !in.eof(); ++i)
        {   
            loadQuestion(in, loadedQuestions.array[i]);
            // move past the '|' deliminator
            in.get();
            // move past the '\n' deliminator
            in.get();
        }
        // close out the file 
        in.close();
        // clear the connection
        in.clear();
        // add the size of the questions loaded
        // there will be an extra empty question because one will have to be read in
        // to trigger the .eof() flag, but we dont count it
        loadedQuestions.size = i - 1;
        cout << i - 1 << " questions loaded!" << endl;
    }
    return;
}



// loads a single question from a passed ifstream
void loadQuestion(ifstream& in, question& question)
{
    // get the question number
    in >> question.number;
    // move past '\n' deliminator
    in.get();
    // get the date
    in.get(question.date, MAX_DATE_SIZE, '\n');
    // move past '\n' deliminator
    in.get();
    // get the subject number
    in >> question.subject;
    // move past '\n' deliminator
    in.get();
    // get the text
    in.get(question.text, MAX_QUESTION_SIZE, '\n');
    // move past '\n' deliminator
    in.get();
}



// saves questions entered to external file
void saveQuestions(structQuestionArray& newQuestions, int totalQuestions)
{
    // get the fstream involved
    ofstream out;
    // because we have to
    int i;
    out.open(QUESTION_TOTAL_FILE);
    if(out)
    {
        // overwrite the current total question number
        // to add the amount that will be saved
        // add a newline character so we stop there when reading back in
        out << totalQuestions + newQuestions.size << endl;
        // close out the file 
        out.close();
        // clear the connection
        out.clear();
    }
    // open the file
    out.open(FILE_NAME, ios::app);
    // if we connect do the thing
    if(out)
    {
        for(i = 0; i < newQuestions.size; i++) 
        {
            // endl is the line deliminator
            out << newQuestions.array[i].number << endl;
            out << newQuestions.array[i].date << endl;
            out << newQuestions.array[i].subject << endl;
            out << newQuestions.array[i].text << endl;
            // add the question deliminator
            out << '|' << endl;
        }

        cout << "Saving " << newQuestions.size << " question(s) to " << FILE_NAME << " successful!" << endl;
        // close out the file 
        out.close();
        // clear the connection
        out.clear();
    }
    return;
}



// displays all loaded questions
void displayAllQuestions(totalQuestionArray& questions)
{
    int i;
    for(i = 0; i < questions.size; ++i) displayQuestion(questions.array[i]);
    cout << i << " questions displayed!" << endl;
    return;
}



// displays given question in terminal
void displayQuestion(question& question)
{
    cout << "Number: " << question.number << endl;
    cout << "Date Added: " << question.date << endl;
    cout << "Subject: " << subjects[question.subject].name << endl;
    cout << "Text: " << question.text << endl;
    cout << "_________________________" << endl;
    cout << endl;
    return;
}

// obtains user input to construct questions
// and add to given newQuestions array
// returns a boolean
void createQuestions(structQuestionArray& newQuestions, int totalQuestions)
{
    // count for the questions
    int count = 0;
    // while condition
    bool addQuestion = true;
    do 
    {
        getQuestion(newQuestions.array[count]);
        count++;
        // add the question number to the question
        // we do this after count is increased so the question is actually counted
        // since count starts at zero
        newQuestions.array[count - 1].number = count + totalQuestions;
        cout << "You've entered " << count << "/10 new questions" << endl;
        if(count < MAX_NEW_QUESTIONS + 1) 
        {
            cout << "Would you like to enter another question? (y/n): ";
            addQuestion = getResponse();
            if(addQuestion) cout << endl;
        }
        else cout << "You've entered the max amount of questions!" << endl;
    // while the user wants to add a question and the total amount is less than the max of 10
    } while(addQuestion && count < MAX_NEW_QUESTIONS + 1);
    newQuestions.size = count;
    return;
}



// takes in a question struct and promps the user to enter in 
// responses for the question contents
void getQuestion(question& question)
{
    question.subject = getSubject();
    cout << "Enter question text: ";
    // obtain question text.
    getString(question.text, MAX_QUESTION_SIZE);
    cout << "Enter today's date (xx/xx/xxxx): ";
    // obtain the date from the user and set the size with the return value
    getString(question.date, MAX_DATE_SIZE);
    return;
}



// displays the main menu and obtains the users
// response to what they want to do
int getMainMenu()
{
    // an integer for the highest possible response
    int maxResponse = 5;
    // an integer for the lowest possible response
    int minResponse = 0;
    cout << "~~~~~~~~~~ Main Menu ~~~~~~~~~~" << endl;
    cout << "0. Quit program" << endl;
    cout << "1. Create new practice questions" << endl;
    cout << "2. Save created practice questions" << endl;
    cout << "3. Load questions from external drive" << endl;
    cout << "4. Display all saved questions" << endl;
    cout << "5. Display all saved questions of a certain subject" << endl;
    cout << "Enter a corresponding number to preform a task: ";

    return getIntegerRangeResponse(maxResponse, minResponse);
}


// obtains an integer response
// between a certain inclusive range
int getIntegerRangeResponse(int max, int min)
{
    int response;
    while(!(cin >> response)
            // if not greater than or equal to min
            || !(response >= min
            // or less than or equal to max
            && response <= max))
    {
        cout << "Invalid input, try again: ";
        clearBufferStream();
    }
    clearBufferStream();
    return response;
}



// a helper function for cleaning out the buffer
// since we will be doing it multiple times
void clearBufferStream(void)
{
   // clear out the errorstate
   // just in case it exists
   cin.clear();
   // ignore the rest of what is in the stream
   // up to 200 characters or
   // until carriage return is hit '\n'
   // and we can flush out the carriage return
   cin.ignore(200,'\n');
   return;
}

// this helper obtains characters and places them into an array
// via array reference
// placement happens insde the while loop expression
// since arrays use references, nothing needs to be returned
// while also validating proper length of input
// it takes an array reference to place the string
// as well as the array size desired
int getString(char array[], int arraySize)
{
  // if the buffer stream is in error state
  while(!cin.get(array, arraySize, '\n')
          // or the next character in the buffer is not a carriage return
          || cin.peek() != '\n')
  {
          // insert a break so we are not on the same line as "enter phrase number...:"
          cout << endl;
          if(cin.peek() != '\n') cout << "You entered too many characters, please try again: ";
          else cout << "Invalid input, please try again: ";
          // clear out that dirty stream
          clearBufferStream();
  }
  // clear out that dirty stream
  clearBufferStream();
  // return the length of the array since we can measure it here and will need it later
  return strlen(array);
}

// this helper obtains a single character
// checks if it is a valid y / n response
// and returns a boolean based on if the response
// is yes = true, no = false
bool getResponse(void)
{
  char response = '\0';
  // if the buffer stream is in error state
  while(!cin.get(response)
          // if response is not equal to y
          || (tolower(response) != 'y'
              // if the response is not equal to n
              && tolower(response) != 'n')
          // or the next character in the buffer is not a carriage return
          || cin.peek() != '\n')
  {
          if(cin.peek() != '\n') cout << "You entered too many characters, please try again: ";
          else cout << "Invalid input, please try again: ";
          // clear out that dirty stream
          clearBufferStream();
  }
          // clear out that dirty stream
          clearBufferStream();
  // return a boolean
  return tolower(response) == 'y';
}
