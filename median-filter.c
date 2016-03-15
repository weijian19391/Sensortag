/*Algorithm obtained from Phil Ekstrom (Embedded Systems Programming, November 2000)*/

#include <stdio.h>
#define WINDOW_SIZE 7 /* Size of the data buffer; length of the sequence. */
#define STOPPER 0 /* Smaller than any reading */
static unsigned int medfilter(unsigned int reading)
{
    unsigned int i;
    struct pair
    {
        struct pair   *point;  /* Pointers forming list linked in sorted order */
        unsigned int  value;  /* Values to sort */
    };
    static struct pair buffer[WINDOW_SIZE];  /* Array of pairs with size == WINDOW_SIZE */
    static struct pair *datpoint={buffer}  /* pointer into circular buffer of data */;
    static struct pair small={NULL,STOPPER} ;  /* chain stopper. */
    static struct pair big={&small,0} ;  /* pointer to head (largest) of linked list.*/
    struct pair *successor   ;  /* pointer to successor of replaced data item */
    struct pair *scan        ;  /* pointer used to scan down the sorted list */
    struct pair *scanold     ;  /* previous value of scan */
    struct pair *median;     ;  /* pointer to median */

    if(reading == STOPPER) reading = STOPPER + 1; /* No stoppers allowed. */
    if( (++datpoint - buffer) >= WINDOW_SIZE) datpoint=buffer;  /* increment and wrap data in pointer.*/
    datpoint->value=reading        ;  /* Copy in new reading */
    successor=datpoint->point    ;  /* save pointer to old value's successor */
    median = &big                ;  /* median initially to first in chain */
    scanold = NULL               ;  /* scanold initially null. */
    scan = &big                  ;  /* points to pointer to first (largest) reading in chain */
  /* Handle chain-out of first item in chain as special case */
        if( scan->point == datpoint ) scan->point = successor;
        scanold = scan ;            /* Save this pointer and   */
        scan = scan->point ;        /* step down chain */
  /* loop through the chain, normal loop exit via break. */
    for( i=0 ;i<WINDOW_SIZE ; i++ )
    {
     /* Handle odd-numbered item in chain  */
        if( scan->point == datpoint ) scan->point = successor;  /* Chain out the old reading.*/
        if( (scan->value < reading) )        /* If reading is larger than scanned value,*/
        {
            datpoint->point = scanold->point;          /* chain it in here.  */
            scanold->point = datpoint;          /* mark it chained in. */
            reading = STOPPER;
        };
  /* Step median pointer down chain after doing odd-numbered element */
        median = median->point       ;       /* Step median pointer.  */
        if ( scan == &small ) break ;        /* Break at end of chain  */
        scanold = scan ;          /* Save this pointer and   */
        scan = scan->point ;            /* step down chain */
  /* Handle even-numbered item in chain.  */
        if( scan->point == datpoint ) scan->point = successor; 
        if( (scan->value < reading) )         
        {
            datpoint->point = scanold->point;       
            scanold->point = datpoint;
            reading = STOPPER;
        };
        if ( scan == &small ) break;
        scanold = scan ;                            
        scan = scan->point;
    };
    return( median->value );
}
