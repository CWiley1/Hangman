#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

void clearboard();
void populate_vec(int size, vector<string>& base);
void drawboard(char gallows[], char word[], int length);
bool user_interact(char gallows[], char word[], int length, vector<string>& word_base_, int& messups);
char guesser(vector<string>& word_base_, char word[], int length);
void position_rewrite(vector<string>& word_base_, int positions[], int number_of_letters, char current_guess, int length);
void char_rewrite(vector<string>& word_base_, char current_guess);
bool won(char word[], int length);
char hanger(char gallows[], int messups);
void delete_s(vector<string>& mavec, int index);
void print(vector<string> mavec)
{
    for (int i = 0; i < mavec.size(); i++)
    {
        cout << mavec[i] << " ";
    }
    cout << endl;
}

//program

int main()
{
    char gallows[6];
    gallows[0] = ' ';
    gallows[1] = ' ';
    gallows[2] = ' ';
    gallows[3] = ' ';
    gallows[4] = ' ';
    gallows[5] = ' ';

    vector<string> word_base;
    cout << "WELCOME TO HANGMAN";
    cout << endl;
    int word_size = 0;
    cout << "How many letters in your word? ";
    cin >> word_size;
    populate_vec(word_size, word_base);
    char word[word_size];

    int messups = 0;

    for (int i = 0; i < word_size; i++)
    {
        word[i] = ' ';
    }
    drawboard(gallows, word, word_size);

    bool have_won = false;
    while (!have_won)
    {
    char input = ' ';
    char current_guess = guesser(word_base, word, word_size);
    cout << "Is " << current_guess << " in the word?";
    cout << endl;
    cin >> input;
    if (input == 'y')
    {
        int number_of_letters = 0;
        cout << "How many of this letter is in the word? ";
        cout << endl;
        cin >> number_of_letters;
        clearboard();
        drawboard(gallows, word, word_size);
        int positions[number_of_letters];
        for (int i = 0; i < number_of_letters; i++)
        {
            int current_index = 0;
            cout << "Enter position of letter " << i + 1 << endl;
            cin >> current_index;
            positions[i] = current_index - 1;
            word[current_index - 1] = current_guess;
        }
        position_rewrite(word_base, positions, number_of_letters, current_guess, word_size);
    }
    else if (input == 'n')
    {
        char_rewrite(word_base, current_guess);
        messups++;
        hanger(gallows, messups);
    }
        char ans = ' ';

        if (word_base.size() == 1)
        {
            cout << "Is " << word_base[0] << " your word? (y/n)" << endl;
            cin >> ans;
        }

        if (ans == 'y')
        {
            have_won = true;
            for (int i = 0; i < word_size; i++)
            {
                word[i] = word_base[0][i];
            }
        }

        if(won(word, word_size) || messups > 6)
        {
            have_won = true;
        }
        clearboard();
        drawboard(gallows, word, word_size);

    }


    if(messups > 6)
    {
        cout << "You win!" << endl;
        print(word_base);

    }
    else
    {
        cout << "I win!" << endl;
    }


    return 0;
}
void populate_vec(int size, vector<string>& base)
{

    ifstream in_stream;
    in_stream.open("/Users/Wiley/Desktop/wordsEn.txt");
    if (in_stream.fail()) {
        cout << "Failed to open input.txt" << endl;
        exit(1);
    }
    string current = "";
    while(current != "zyzzyvas")
    {
        in_stream >> current;
        if (current.size() == size)
        {
            base.push_back(current);
        }
    }


    in_stream.close();


}

void drawboard(char gallows[],char word[], int length)
{

    cout << "|-------|" << endl;
    cout << "|       " << gallows[0] << endl;
    cout << "|      " << gallows[2] << gallows [1] << gallows [3] << endl;
    cout << "|      " << gallows[4] << " " << gallows[5] << endl;
    cout << "|\n";
    cout << "---------" << endl;
    cout << endl << endl << endl;

    for (int k = 0; k < length; k++)
    {
        cout << " " << word[k] << " ";
    }
    cout << endl;
    for (int i = 0; i < length; i++)
    {
        cout << " - ";
    }
    cout << endl;
    for (int j = 0; j < length; j++)
    {
        cout << " " << j+1 << " ";
    }
    cout << endl;

}

void clearboard()
{
    for (int i = 0; i < 20; i++)
    {
        cout << endl;
    }
}


char guesser(vector<string>& word_base_, char word[], int length)
{

    map<char, int> char_count;

    for (int i = 0; i < length; i++)
    {
        char_count[word[i]] = -9999;
    }

    for (int i = 0; i < word_base_.size(); i++)
    {
       // cout << word_base_[i] << endl;
        for (int j = 0; j < length; j++)
        {
            char_count[word_base_[i][j]]++;
        }
    }

    int max = 0;
    char max_ = ' ';

    for (auto x : char_count)
    {
        if (x.second >= max)
        {
            max = x.second;
            max_ = x.first;
        }
    }

    return max_;

}


void position_rewrite(vector<string>& word_base_, int positions[], int number_of_letters, char current_guess, int length)
{

    bool erase = false;
    int count = 0;

    for (int i = 0; i < word_base_.size(); i++)
    {

        erase = false;
        count = 0;

        for (int j = 0; j < number_of_letters; j++)
        {

            if (word_base_[i][positions[j]] != current_guess)
            {
                j+= 100;
                erase = true;
            }

            if (erase)
            {
                delete_s(word_base_, i);
                i--;
            }

        }


    }

    for (int i = 0; i < word_base_.size(); i++)
    {

        count = 0;
        erase = false;
        for (int c = 0; c < length; c++)
        {
            if (word_base_[i][c] == current_guess)
            {
                count++;
            }
        }
        if (count != number_of_letters)
        {
            erase = true;
        }
        if (erase)
        {
        delete_s(word_base_, i);
        i--;
        }
    }
}
void char_rewrite(vector<string>& word_base_, char current_guess)
{
    int length = word_base_[0].length();
    for(int i = 0; i < word_base_.size(); i++)
    {
        for(int j = 0; j < length; j++)
        {
            if (word_base_[i][j] == current_guess)
            {
                delete_s(word_base_, i);
                j += 100;
                i--;
            }
        }
    }
}

bool won(char word[], int length)
{

    int counter = 0;
    for (int i = 0; i < length; i++)
    {
        if (word[i] != ' ')
        {
            counter++;
        }

    }
    if (counter == length)
    {
        return true;
    }
    else
    {
        return false;
    }

}

char hanger(char gallows[], int messups)
{
    if (messups == 1)
    {
        gallows[0] = 'O';
        return gallows[0];
    }
    else if (messups == 2)
    {
        gallows[1] = '|';
        return gallows[1];
    }
    else if (messups == 3)
    {
        gallows[2] = '-';
        return gallows[2];
    }
    else if (messups == 4)
    {
        gallows[3] = '-';
        return gallows[3];
    }
    else if (messups == 5)
    {
        gallows[4] = '/';
        return gallows[4];
    }
    else if (messups == 6)
    {
        gallows[5] = '\\';
        return gallows[5];
    }
    else if (messups > 6)
    {

        cout << "You Lose." << endl;
        return -1;
    }
    return gallows[3];

}

void delete_s(vector<string>& mavec, int index)
{
    if (index == mavec.size() - 1)
    {
        mavec.resize(mavec.size() - 1);
    }
    else
    {
        mavec[index] = mavec[mavec.size() - 1];
        mavec.resize(mavec.size() - 1);
    }
}
