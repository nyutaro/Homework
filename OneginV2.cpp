#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <locale>

#define FILENAME "test.txt"
#define NEWFILE "testResult.txt"
#define NEWFILEFORBACK "TestResultback.txt"

const char punct[] = ",.() /;:\"\\-!?><1234567890'";

const int LESS = -1;
const int MORE = 2;
const int TEARS = 1;
const int LUCKY = 0;
const int EQUAL = 0;

struct String
{
    char* str;
    int len;
};

void OneginSort1(String** text, int nStrings,
                    int (*comp)(const String*, const String*));

const char* readfile(const char* filename, size_t* len);
String** makeStrings(size_t* nStrings);
int writefile(const char* filename, size_t nStrings, String** text);
bool destructStringArr(String** text, size_t nStrings);
bool destructStr(String* This);
String** makeStringBuffer(String** text, size_t nStrings);
void OneginSort(int (*comp) (const String*, const String*),
                        String** text, int left, int right);
int Strcmp(const String* str1, const String* str2);
int backStrcmp(const String* str1, const String* str2);
bool string_dump(String** text,  size_t nStrings);
void printBuffer(const char* buffer, size_t length);
void Swap (String* str1, String* str2);


int main()
{
    printf("# ---Onegin sort by nyuta\n");

    size_t nStrings = 0;
    String** Onegin =  makeStrings(&nStrings);
        if (!Onegin)
        return perror("You're wrong, mate"), TEARS;
        string_dump(Onegin, nStrings);

    String** Oneginbuf  = makeStringBuffer(Onegin, nStrings);
        if (!Oneginbuf)
        return perror("You're wrong again, mate"), TEARS;

    OneginSort(Strcmp, Onegin, 0, nStrings - 1);
    writefile(NEWFILE, nStrings, Onegin);

    OneginSort(backStrcmp, Onegin, 0, nStrings - 1);
    writefile(NEWFILEFORBACK, nStrings, Onegin);
    string_dump(Onegin, nStrings);


    if (NULL) {
    if (destructStringArr(Onegin, nStrings))
        return perror("You're wrong about destructing Onegin, mate"), TEARS;

    if (destructStringArr(Oneginbuf, nStrings))
        return perror("You're wrong about destructing Oneginbuf, mate"), TEARS;
        }


return LUCKY;
}


bool string_ok(const String* This)
{
return This && This->str && This->len >= 0;
}


bool string_dump(String** text, size_t nStrings)
{
    printf("# --String_dump <<\n");
    for (int i = 0; i < nStrings; i++)
    {   printf("text[%d]", i);

        if (string_ok(text[i]))
             printf("(ok)\n"
            "{\n"
            "str = <<%s>>\n"
            "len = %d\n"
            "}\n\n",
            text[i]->str, text[i]->len
            );
        else
        {  printf("BUG\n");
            if (!text[i])
            {
                printf("{\n"
                "pointer to text[%d] fault\n"
                "}\n\n", i);
            }
            if (!text[i]->str)
            {
                printf("{\n"
                "pointer to text[%d]->str fault\n"
                "}\n\n", i);
            }
            if (text[i]->len < 0)
             {
                printf("{\n"
                "size of len fault\n"
                "}\n\n");
            }

        }
    }
    printf(">> End of dump");
return LUCKY;
}

String** makeStringBuffer(const String** text, size_t nStrings)
{
    String** Buffer = (String**)calloc(nStrings, sizeof(*Buffer));
    if (!Buffer)
    {
     errno = ENOMEM;
     return 0;
    }
    for (int i = 0; i < nStrings; i++)
    {
    assert(i < nStrings && i >= 0);
    Buffer[i]->len = text[i]->len;
    Buffer[i]->str = text[i]->str;
    }
    if (!Buffer) return 0;
return Buffer;
}


void printBuffer(const char* buffer, size_t length)
    {
        printf("test<");

        for (size_t i = 0; i < length; i ++)
        {
            assert(i >=0 && i < length);

            if  (buffer[i] == '\0')
                printf ("#");
            else if  (buffer[i] == '\n')
                printf ("$");
            else
                printf ("%c", buffer[i]);

        }
    printf (">");
}


const char* readfile(const char* filename, size_t* len)
{
    FILE* f = fopen(filename, "rb");
        if (!f)
        {
            errno = ENOENT;
            return 0;
        }
    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    rewind(f);

    const char* buf = (const char*)calloc (*len, sizeof(*buf));
        if (!buf)
        {
            errno = ENOMEM;
            return 0;
        }

    if (!fread((char*)buf, sizeof(*buf), *len, f))
        {
            errno = ENOTTY;
            return 0;
        }
    fclose(f);

return buf;
}


String** makeStrings(size_t* nStrings)
{

   // getchar();
    size_t len = 0, textSize = 0;
    bool isWindows = 0;
    char* buf = (char*)readfile(FILENAME, &len);
        if (!buf) return 0;

    for (int i = 0; i < len - 1; i++)
    {
        assert (i >= 0 && i < len - 1);
        assert (i + 1 >= 0 && i + 1 < len);
        if (buf[i] == '\n')
        {
        textSize++;
       // printf("<%d> ", textSize);
        //getchar();
        }
        if ((buf[i] == '\r') && (buf[i + 1] == '\n'))
        isWindows = 1;
    }
    textSize++;
    *nStrings = textSize;
   // printf("<%d>", textSize);
    //getchar();
    String** text = (String**)calloc(textSize, sizeof(*text));
        if (!text)
        {
            errno = ENOMEM;
            return 0;
        }
    String* strings = (String*)calloc(textSize, sizeof(*text));
        if (!strings)
        {
            errno = ENOMEM;
            return 0;
        }

    for (int i = 0; i < textSize; ++i)
        {
            assert(i >= 0 && i < textSize);
            text[i] = &strings[i];
          //  printf("<text[%d] = %d>", i, text[i]);
            //getchar();
        }

    int j = 0;
    int k = 0;
    text[0]->str = &buf[0];
    //printf("<<%d>>", text[0]->str);
    //getchar();

    for (int i = 0; i < len - 1; i++)
    {
        assert(j >= 0 && j < textSize);
        assert(i >= 0 && i < len - 1);
         //printf("Bug <i = %d,%c>", i, buf[i]);
          //getchar();
        if (buf[i] != '\n') k++;
        if (buf[i] == '\r') buf[i] = '\0';

        if (buf[i] == '\n')
        {
          buf[i] = '\0';
          assert(j < textSize && j >= 0);
          text[j]->len = k;// printf("<<%d:%d>>", j, k);
          //getchar();
          text[j + 1]->str = &buf[i + 1]; //printf("<str:%d>", i + 1);
      //    printf ("BEFORE:Counter: %d, pointer: %p\n", j, text[j]->str);
          j++; //printf("<After j = %d, i = %d, len = %d>", j, i, len - 1);
          //getchar();
          k = 0;
        }
      //  printf ("AFTER:Counter: %d, pointer: %p\n", j, text[j]->str);
    }
    text[textSize - 1]->len = k;
    //printf("<<<%d %d>\n",text[textSize - 1]->len, textSize);
    //printf("nStrings: %d; text[j] = %s", *nStrings, text[textSize - 1]->str);
return text;
}


int backStrcmp(const String* str1, const String* str2)
{
    assert(string_ok(str1));
    assert(string_ok(str2));

    if (str1->len == 0 && str2->len >  0) return LESS;
    if (str1->len >  0 && str2->len == 0) return MORE;
    if (str1->len == 0 && str2->len == 0) return EQUAL;

    int i = str1->len - 1, j = str2->len - 1;

    for (;  strchr(punct, str1->str[i]) && i > 0; i--);
    for (;  strchr(punct, str2->str[i]) && j > 0; j--);

    for (;i != 0  && j != 0;)
        {
            assert(i < str1->len && i >=0);
            assert (j < str2->len && j >=0);
            if (strchr(punct, str1->str[i])) i--;
            if (strchr(punct, str2->str[j])) j--;
            if ((!strchr(punct, str1->str[i])) && (!strchr(punct, str2->str[j])))
            {
                if (str1->str[i] == str2->str[j])
                {
                    i--;
                    j--;
                }
                else return str1->str[i] - str2->str[j];
            }
        }
if (i == 0 && j > 0) return LESS;
if (i > 0 && j == 0) return MORE;
if (i == 0 && j == 0) return EQUAL;
}


int Strcmp(const String* str1, const String* str2)
{
    assert(string_ok(str1));
    assert(string_ok(str2));
     int i = 0, j = 0;
     if (i == str1->len - 1 && j < str2->len - 1) return LESS;
     if (i < str1->len - 1 && j == str2->len - 1) return MORE;
     if (i == str1->len - 1 && j == str2->len - 1 ) return EQUAL;

           for (;i < str1->len && j < str2->len;)
        {
            assert(i < str1->len && i >= 0);
            assert(j < str2->len && j >= 0);
            if (i == str1->len - 1 && j < str2->len - 1) return LESS;
            if (i < str1->len - 1 && j == str2->len - 1) return MORE;
            if (strchr(punct, str1->str[i])) i++;
            if (strchr(punct, str2->str[j])) j++;
            if ((!strchr(punct, str1->str[i])) && (!strchr(punct, str2->str[j])))
            {
                if (str1->str[i] == str2->str[j])
                {
                    i++;
                    j++;
                }
                else  return str1->str[i] - str2->str[j];
            }
        }
    if (i == str1->len - 1 && j == str2->len - 1 ) return EQUAL;
}

/*void OneginSort1(String* text, int nStrings, int (*comp) (const String*, const String*))
{
        for (int i = 0; i < nStrings - 1; ++i)
        {
        for (int j = 0; j < nStrings - i - 1; ++j)
            {
            assert (i >= 0 && i < nStrings);
            assert (i + 1 >= 0 && i + 1 < nStrings);
            assert (j >= 0 && j < nStrings);
            assert (j + 1 >= 0 && j + 1 < nStrings);

           if (comp(&text[j], &text[j + 1]) > 0)
              Swap(&text[j], &text[j + 1]);

        }
    }
}*/


void OneginSort(int (*comp) (const String*, const String*),
                         String** text, int left, int right)
{
    int i = 0, last = 0;
    if (left >= right)
        return;

    Swap(text[left], text[(left + right) / 2]);
    last = left;

    for (i = left + 1; i <= right; ++i)
    {
        assert(i >= 0 && i <= right);
        if (comp(text[i], text[left]) < 0)
            Swap(text[++last], text[i]);
    }
    assert(left >= 0 && last >= 0);
    Swap(text[left], text[last]);
    OneginSort(comp, text, left, last -1);
    OneginSort(comp, text, last + 1, right);

}

void Swap (String* a, String* b)
{
    assert(a);
    assert(b);
    String temp;
    temp.len = a->len;
    temp.str = a->str;
    a->len = b->len;
    a->str = b->str;
    b->len = temp.len;
    b->str = temp.str;
}

int writefile(const char* filename, size_t nStrings, String** text)
{
    assert(text);
    assert(filename);
    FILE* NewFile = fopen(filename, "wb");
        if (!NewFile)
        {
        errno = EIO;
        return TEARS;
        }

   for (int i = 0; i < nStrings; ++i)
        {
            assert(i >= 0 && i < nStrings);
            fprintf(NewFile, "%s\n", text[i]->str);
        }
        fclose(NewFile);

return LUCKY;
}

bool destructStr(String* This)
{
    if (!string_ok(This))
    {
        errno = EIDRM;
        return TEARS;

    }
    else
    {
        free(This->str);
        This->str = 0;
        This->len = -1;
    }

return LUCKY;
}


bool destructStringArr (String** text, size_t nStrings)
{
    if (!text)
    {
        errno = EIDRM;
        return TEARS;
    }
    for (int i = 0; i < nStrings; i++)
    {
        assert(i >= 0 && i < nStrings);
        if (destructStr(text[i])) return TEARS;
    }
    text = 0;

return LUCKY;
}

