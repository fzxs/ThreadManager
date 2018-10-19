
#ifndef __BITHELPER_H_
#define __BITHELPER_H_

#define BIT_ENABLED(WORD,BITS) (((WORD)&(BITS)) != 0)
#define BIT_CMP_MASK(WORD,BITS,MASK) (((WORD)&(BITS)) == MASK)
#define SET_BITS(WORD,BITS) ((WORD) |= (BITS))
#define CLR_BITS(WORD,BITS) ((WORD) &= (~(BITS)))

#endif



