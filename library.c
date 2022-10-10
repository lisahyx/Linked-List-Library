// Lisa Ho Yen Xin 20297507
// completed using Replit (changed from clang to gcc) & onlineGDB (gcc)
// delete function - second last slot problem
// list function - changes the order of the books in original collection during sorting

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define strlength 49
#define booknum 30

// struct to store information about one book
struct book
{
    char *title;
    char *author;
    char *subject;
};

// struct to store information about the library collection
struct library
{
    struct book collection;
    int num_books;
    struct library *next;
};

// list of functions used
void copybook(struct book *dest, struct book *source);
void addbook(FILE *in, FILE *out, struct library **thislib);
void deletetitle(FILE *in, FILE *out, struct library **thislib);
void deleteauthor(FILE *in, FILE *out, struct library **thislib);
void searchtitle(FILE *in, FILE *out, struct library **thislib);
void searchauthor(FILE *in, FILE *out, struct library **thislib);
void listauthor(FILE *in, FILE *out, struct library **thislib);
void listsubject(FILE *in, FILE *out, struct library **thislib);
void listbook(FILE *out, struct library *thislib);

int main(void)
{
    FILE *in;  // in = library.txt
    FILE *out; // out = output.txt

    struct library *mylibrary = NULL;

    mylibrary = (struct library *)malloc(sizeof(struct library));
    mylibrary->collection = *(struct book *)malloc(sizeof(struct book));
    mylibrary->collection.title = (char *)malloc(sizeof(strlength));
    mylibrary->collection.author = (char *)malloc(sizeof(strlength));
    mylibrary->collection.subject = (char *)malloc(sizeof(strlength));

    // open file, exit if unable to open
    if ((in = fopen("library.txt", "r")) == NULL || (out = fopen("output.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
        exit(1);
    }

    // reads the number of updates to the library
    int updates;
    fscanf(in, "%d", &updates);

    for (int i = 0; i < updates; i++)
    {
        // reads the operations from input file
        int operation;
        fscanf(in, "%d", &operation);

        switch (operation)
        {
        case 1:
            addbook(in, out, &mylibrary);
            break;

        case 2:
            deletetitle(in, out, &mylibrary);
            break;

        case 3:
            deleteauthor(in, out, &mylibrary);
            break;

        case 4:
            searchtitle(in, out, &mylibrary);
            break;

        case 5:
            searchauthor(in, out, &mylibrary);
            break;

        case 6:
            listauthor(in, out, &mylibrary);
            break;

        case 7:
            listsubject(in, out, &mylibrary);
            break;

        case 8:
            listbook(out, mylibrary);
            break;
        }
    }
    fclose(in);
    fclose(out);

    return 0;
}

// copies the contents of one book into another book
void copybook(struct book *dest, struct book *source)
{
    if (source != NULL)
    {
        strcpy(dest->title, source->title);
        strcpy(dest->author, source->author);
        strcpy(dest->subject, source->subject);
    }
}

// add a book to the collection (must have complete information, title, author and subject)
void addbook(FILE *in, FILE *out, struct library **thislib)
{
    struct library *newptr;
    struct library *currentptr;
    struct library *previousptr;

    newptr = (struct library *)malloc(sizeof(struct library));
    newptr->collection.title = (char *)malloc(sizeof(strlength));
    newptr->collection.author = (char *)malloc(sizeof(strlength));
    newptr->collection.subject = (char *)malloc(sizeof(strlength));

    fscanf(in, "%48s %48s %48s", newptr->collection.title, newptr->collection.author, newptr->collection.subject); // cannot exceed 49 characters

    if (newptr != NULL) // if memory available
    {
        newptr->next = NULL;
        previousptr = NULL;
        currentptr = *thislib;

        while (currentptr != NULL) // loop through the whole collection to check if book exists
        {
            if (strcmp(currentptr->collection.title, newptr->collection.title) == 0) // if book already exists
            {
                printf("\nThe book %s is currently in the library.\n", newptr->collection.title);
                fprintf(out, "\nThe book %s is currently in the library.\n", newptr->collection.title);
                return;
            }
            previousptr = currentptr;
            currentptr = currentptr->next;
        }

        if (previousptr == NULL) // if collection is empty, insert book at the beginning of the collection
        {
            newptr->next = *thislib;
            *thislib = newptr;
        }
        else // if not, insert at the end
        {
            previousptr->next = newptr;
            newptr->next = currentptr;
        }

        copybook(&previousptr->collection, &newptr->collection); // copy book into collection

        printf("\nThe book %s author %s subject %s has been added to the library.\n", newptr->collection.title, newptr->collection.author, newptr->collection.subject);
        fprintf(out, "\nThe book %s author %s subject %s has been added to the library.\n", newptr->collection.title, newptr->collection.author, newptr->collection.subject);

        (*thislib)->num_books += 1; // number of books increase by 1

        if ((*thislib)->num_books >= 31) // if number of books exceed 30
        {
            printf("\nYour library cannot exceed 30 books.\n");
        }
    }
    else // if no memory available
    {
        puts("\nBook not added. No available memory.\n");
        fprintf(out, "\nBook not added. No available memory.\n");
        return;
    }
}

// delete a book from the collection by a particular title
void deletetitle(FILE *in, FILE *out, struct library **thislib)
{
    struct library *deletethis; // the book to delete
    struct library *currentptr;
    struct library *previousptr;
    struct library *tempptr;
    struct library *secondlast; // the second last node

    deletethis = (struct library *)malloc(sizeof(struct library));
    deletethis->collection.title = (char *)malloc(sizeof(strlength));
    currentptr = (struct library *)malloc(sizeof(struct library));
    currentptr->collection.title = (char *)malloc(sizeof(strlength));
    previousptr = (struct library *)malloc(sizeof(struct library));
    previousptr->collection.title = (char *)malloc(sizeof(strlength));
    secondlast = (struct library *)malloc(sizeof(struct library));
    secondlast->collection.title = (char *)malloc(sizeof(strlength));
    tempptr = (struct library *)malloc(sizeof(struct library));
    tempptr->collection.title = (char *)malloc(sizeof(strlength));

    fscanf(in, "%48s", deletethis->collection.title);

    previousptr = NULL;
    currentptr = *thislib;

    if (strcmp(currentptr->collection.title, deletethis->collection.title) == 0) // if book is in first node
    {
        printf("\nThe book %s has been removed from the library.\n", currentptr->collection.title);
        fprintf(out, "\nThe book %s has been removed from the library.\n", currentptr->collection.title);

        tempptr = currentptr->next; // this is the book after the deleted book

        // find second last node
        while (currentptr->next->next != NULL)
        {
            secondlast = currentptr;
            currentptr = currentptr->next;
        }

        *thislib = secondlast; // replace with second last node
        (*thislib)->next = tempptr;

        while (currentptr->next != NULL)
        {
            (*thislib)->next->next = currentptr;
            currentptr = currentptr->next;
        }

        currentptr->next = NULL; // initialise last node to NULL

        (*thislib)->num_books -= 1;
        return;
    }
    else // loop through the collection to find the book
    {
        previousptr = *thislib;
        currentptr = (*thislib)->next;

        while (currentptr->next != NULL && strcmp(currentptr->collection.title, deletethis->collection.title) != 0)
        {
            previousptr = currentptr;
            currentptr = currentptr->next;
        }

        if (currentptr->next != NULL)
        {
            printf("\nThe book %s has been removed from the library.\n", currentptr->collection.title);
            fprintf(out, "\nThe book %s has been removed from the library.\n", currentptr->collection.title);

            tempptr = currentptr->next; // this is the book after the deleted book

            // find second last node
            while (currentptr->next->next != NULL)
            {
                secondlast = currentptr;
                currentptr = currentptr->next;
            }

            previousptr->next = secondlast;    // replace with second last node
            previousptr->next->next = tempptr; // the book after the deleted book
            currentptr->next = NULL;           // initialise last node to NULL

            (*thislib)->num_books -= 1;

            return;
        }
        else
        {
            printf("\nDeletion cannot be performed as the requested item does not exist.\n");
            fprintf(out, "\nDeletion cannot be performed as the requested item does not exist.\n");
            return;
        }
    }
    free(deletethis);
    free(currentptr);
    free(previousptr);
    free(tempptr);
    free(secondlast);
}

// delete all books from the collection by a particular author
void deleteauthor(FILE *in, FILE *out, struct library **thislib)
{
    struct library *deletethis; // the book to delete
    struct library *currentptr;
    struct library *previousptr;
    struct library *tempptr;
    struct library *secondlast; // the second last node

    deletethis = (struct library *)malloc(sizeof(struct library));
    deletethis->collection.author = (char *)malloc(sizeof(strlength));
    currentptr = (struct library *)malloc(sizeof(struct library));
    currentptr->collection.title = (char *)malloc(sizeof(strlength));
    currentptr->collection.author = (char *)malloc(sizeof(strlength));
    previousptr = (struct library *)malloc(sizeof(struct library));
    previousptr->collection.title = (char *)malloc(sizeof(strlength));
    previousptr->collection.author = (char *)malloc(sizeof(strlength));
    secondlast = (struct library *)malloc(sizeof(struct library));
    secondlast->collection.title = (char *)malloc(sizeof(strlength));
    secondlast->collection.author = (char *)malloc(sizeof(strlength));
    tempptr = (struct library *)malloc(sizeof(struct library));
    tempptr->collection.title = (char *)malloc(sizeof(strlength));
    tempptr->collection.author = (char *)malloc(sizeof(strlength));

    fscanf(in, "%48s", deletethis->collection.author);

    previousptr = NULL;
    currentptr = *thislib;

    if (strcasecmp(currentptr->collection.author, deletethis->collection.author) == 0) // if book is in first node
    {
        printf("\nThe book %s authored by %s has been removed from the library.\n", currentptr->collection.title, currentptr->collection.author);
        fprintf(out, "\nThe book %s authored by %s has been removed from the library.\n", currentptr->collection.title, currentptr->collection.author);

        tempptr = currentptr->next; // this is the book after the deleted book

        // find second last node
        while (currentptr->next->next != NULL)
        {
            secondlast = currentptr;
            currentptr = currentptr->next;
        }

        *thislib = secondlast; // replace with second last node

        while (tempptr != secondlast)
        {
            currentptr->next->next = tempptr;
            tempptr = tempptr->next;
        }

        currentptr->next = NULL; // initialise last node to NULL

        (*thislib)->num_books -= 1;
        return;
    }
    else // loop through collection to find the book
    {
        previousptr = *thislib;
        currentptr = (*thislib)->next;

        while (currentptr->next != NULL && strcasecmp(currentptr->collection.author, deletethis->collection.author) != 0)
        {
            previousptr = currentptr;
            currentptr = currentptr->next;
        }

        if (currentptr->next != NULL)
        {
            printf("\nThe book %s authored by %s has been removed from the library.\n", currentptr->collection.title, currentptr->collection.author);
            fprintf(out, "\nThe book %s authored by %s has been removed from the library.\n", currentptr->collection.title, currentptr->collection.author);

            tempptr = currentptr->next; // this is the book after the deleted book

            // find second last node
            while (currentptr->next->next != NULL)
            {
                secondlast = currentptr;
                currentptr = currentptr->next;
            }

            previousptr->next = secondlast;    // replace with second last node
            previousptr->next->next = tempptr; // the book after the deleted book
            currentptr->next = NULL;           // initialise last node to NULL

            (*thislib)->num_books -= 1;

            return;
        }
        else
        {
            printf("\nDeletion cannot be performed as the requested item does not exist.\n");
            fprintf(out, "\nDeletion cannot be performed as the requested item does not exist.\n");
            return;
        }
    }
    free(deletethis);
    free(currentptr);
    free(previousptr);
    free(tempptr);
    free(secondlast);
}

// search for a book from the collection by a particular title
void searchtitle(FILE *in, FILE *out, struct library **thislib)
{
    struct library *currentptr = *thislib;
    struct library *searchthis; // book title to find

    searchthis = (struct library *)malloc(sizeof(struct library));
    searchthis->collection.title = (char *)malloc(sizeof(strlength));

    fscanf(in, "%48s", searchthis->collection.title);

    while (currentptr != NULL && strcmp(currentptr->collection.title, searchthis->collection.title) != 0) // loop through the collection to find the book title
    {
        currentptr = currentptr->next;
    }

    if (currentptr == NULL) // if cannot find book title
    {
        printf("\nThe book %s is NOT currently in the library.\n", searchthis->collection.title);
        fprintf(out, "\nThe book %s is NOT currently in the library.\n", searchthis->collection.title);
        return;
    }
    else // if book title found
    {
        printf("\nThe book %s is currently in the library.\n", currentptr->collection.title);
        fprintf(out, "\nThe book %s is currently in the library.\n", searchthis->collection.title);
    }
    free(searchthis);
}

// search for all books from the collection by a particular author
void searchauthor(FILE *in, FILE *out, struct library **thislib)
{
    struct library *currentptr = *thislib;
    struct library *searchthis; // book author to find

    searchthis = (struct library *)malloc(sizeof(struct library));
    searchthis->collection.author = (char *)malloc(sizeof(strlength));

    int num = (*thislib)->num_books; // number of books in the collection

    fscanf(in, "%48s", searchthis->collection.author);

    while (currentptr != NULL && strcasecmp(currentptr->collection.author, searchthis->collection.author) != 0) // loop through the collection to find the author
    {
        currentptr = currentptr->next;
    }

    printf("\nList of all books by %s\n", searchthis->collection.author);
    fprintf(out, "\nList of all books by %s\n", searchthis->collection.author);

    if (currentptr == NULL) // if cannot find author
    {
        printf("No book from author %s.\n", searchthis->collection.author);
        fprintf(out, "No book from author %s.\n", searchthis->collection.author);
        return;
    }
    else // if author found
    {
        while (num != 0) // continue looping through the collection & print all books by the author
        {
            if (strcasecmp(currentptr->collection.author, searchthis->collection.author) == 0)
            {
                printf("%s\n", currentptr->collection.title);
                fprintf(out, "%s\n", currentptr->collection.title);
            }
            currentptr = currentptr->next;
            num--;
        }
    }
    free(searchthis);
}

// list all of the books by a particular author from the collection in ascending order
void listauthor(FILE *in, FILE *out, struct library **thislib)
{
    struct library *currentptr;
    struct library *nextptr = NULL;
    struct library *tempptr;
    struct library *searchthis; // book author to find

    searchthis = (struct library *)malloc(sizeof(struct library));
    searchthis->collection.author = (char *)malloc(sizeof(strlength));
    currentptr = (struct library *)malloc(sizeof(struct library));
    currentptr->collection.author = (char *)malloc(sizeof(strlength));
    currentptr->collection.title = (char *)malloc(sizeof(strlength));
    nextptr = (struct library *)malloc(sizeof(struct library));
    nextptr->collection.title = (char *)malloc(sizeof(strlength));
    tempptr = (struct library *)malloc(sizeof(struct library));
    tempptr->collection.title = (char *)malloc(sizeof(strlength));

    fscanf(in, "%48s", searchthis->collection.author);

    printf("\nList of all books by %s\n", searchthis->collection.author);
    fprintf(out, "\nList of all books by %s\n", searchthis->collection.author);

    int count = 0; // for checking if the book exits in the collection

    // arrange in ascending order
    for (currentptr = *thislib; currentptr != NULL; currentptr = currentptr->next)
    {
        for (nextptr = currentptr->next; nextptr != NULL; nextptr = nextptr->next)
        {
            if (strcasecmp(currentptr->collection.author, searchthis->collection.author) == 0)
            {
                if (strcmp(currentptr->collection.title, nextptr->collection.title) > 0 && strcmp(currentptr->collection.author, nextptr->collection.author) == 0)
                {
                    strcpy(tempptr->collection.title, currentptr->collection.title);
                    strcpy(currentptr->collection.title, nextptr->collection.title);
                    strcpy(nextptr->collection.title, tempptr->collection.title);
                }
                count++;
            }
        }
    }
    if (count == 0) // if cannot find books by the author
    {
        printf("No book from author %s.\n", searchthis->collection.author);
        fprintf(out, "No book from author %s.\n", searchthis->collection.author);
        return;
    }
    currentptr = *thislib;

    while (currentptr->next != NULL)
    {
        if (strcasecmp(currentptr->collection.author, searchthis->collection.author) == 0)
        {
            printf("%s\n", currentptr->collection.title);
            fprintf(out, "%s\n", currentptr->collection.title);
        }
        currentptr = currentptr->next;
    }
    free(searchthis);
    free(currentptr);
    free(nextptr);
    free(tempptr);
}

// list all of the books by a particular subject from the collection in descending order
void listsubject(FILE *in, FILE *out, struct library **thislib)
{
    struct library *currentptr;
    struct library *nextptr = NULL;
    struct library *tempptr;
    struct library *searchthis; // book subject to find

    searchthis = (struct library *)malloc(sizeof(struct library));
    searchthis->collection.subject = (char *)malloc(sizeof(strlength));
    currentptr = (struct library *)malloc(sizeof(struct library));
    currentptr->collection.subject = (char *)malloc(sizeof(strlength));
    currentptr->collection.title = (char *)malloc(sizeof(strlength));
    nextptr = (struct library *)malloc(sizeof(struct library));
    nextptr->collection.title = (char *)malloc(sizeof(strlength));
    tempptr = (struct library *)malloc(sizeof(struct library));
    tempptr->collection.title = (char *)malloc(sizeof(strlength));

    fscanf(in, "%48s", searchthis->collection.subject);

    printf("\nList of all books by %s\n", searchthis->collection.subject);
    fprintf(out, "\nList of all books by %s\n", searchthis->collection.subject);

    int count = 0; // for checking if the book exits in the collection

    // arrange in descending order
    for (currentptr = *thislib; currentptr != NULL; currentptr = currentptr->next)
    {
        for (nextptr = currentptr->next; nextptr != NULL; nextptr = nextptr->next)
        {
            if (strcasecmp(currentptr->collection.subject, searchthis->collection.subject) == 0) // if book subject found
            {
                if (strcmp(currentptr->collection.title, nextptr->collection.title) < 0 && strcmp(currentptr->collection.subject, nextptr->collection.subject) == 0) // compare title & subject with next book in the collection
                {
                    strcpy(tempptr->collection.title, currentptr->collection.title);
                    strcpy(currentptr->collection.title, nextptr->collection.title);
                    strcpy(nextptr->collection.title, tempptr->collection.title);
                }
                count++;
            }
        }
    }

    if (count == 0) // if cannot find books on the subject
    {
        printf("No related book for %s is found.\n", searchthis->collection.subject);
        fprintf(out, "No related book for %s is found.\n", searchthis->collection.subject);
        return;
    }

    currentptr = *thislib;

    while (currentptr->next != NULL)
    {
        if (strcasecmp(currentptr->collection.subject, searchthis->collection.subject) == 0)
        {
            printf("%s\n", currentptr->collection.title);
            fprintf(out, "%s\n", currentptr->collection.title);
        }
        currentptr = currentptr->next;
    }
    free(searchthis);
    free(currentptr);
    free(nextptr);
    free(tempptr);
}

// list all of the books in the original order
void listbook(FILE *out, struct library *thislib)
{
    puts("\nList all of the books in the original order:");
    fprintf(out, "\nList all of the books in the original order:\n");

    int num = thislib->num_books;

    if (num == 0)
    {
        printf("Library is empty\n");
        fprintf(out, "Library is empty\n");
    }

    while (num != 0)
    {
        printf("%s %s %s\n", thislib->collection.title, thislib->collection.author, thislib->collection.subject);
        fprintf(out, "%s %s %s\n", thislib->collection.title, thislib->collection.author, thislib->collection.subject);
        num -= 1;
        thislib = thislib->next;
    }
}