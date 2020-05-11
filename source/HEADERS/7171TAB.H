/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    7171tab.h                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 15, 1988                                      |
  |                                                                   |
  |  :LEVEL.   VDI                                                    |
  |                                                                   |
  |  :GLOBAL.  xlate7171   byte      OUTPUT  This header file creates |
  |                                          and initializes a global |
  |                                          table.                   |
  |                                                                   |
  |  :PURPOSE. Create a global translate table for the IBM 3270 VDI   |
  |            driver in order to translate 12 bit addressing field   |
  |            characters for non-locally attached IBM terminals.     |
  |            Refer to Appendix D of the GA23-0059-3 IBM Manual.     |
  |                                                                   |
  |                                                                   |
  |  :CALLS.   - none - However, used by vd_xchr().                   |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/




byte xlate7171[ 64 ] =
{
    0x40,
    0xC1,
    0xC2,
    0xC3,
    0xC4,
    0xC5,
    0xC6,
    0xC7,
    0xC8,
    0xC9,
    0x4A,
    0x4B,
    0x4C,
    0x4D,
    0x4E,
    0x4F,

    0x50,
    0xD1,
    0xD2,
    0xD3,
    0xD4,
    0xD5,
    0xD6,
    0xD7,
    0xD8,
    0xD9,
    0x5A,
    0x5B,
    0x5C,
    0x5D,
    0x5E,
    0x5F,
    0x60,
    0x61,
    0xE2,
    0xE3,
    0xE4,
    0xE5,
    0xE6,
    0xE7,
    0xE8,
    0xE9,
    0x6A,
    0x6B,
    0x6C,
    0x6D,
    0x6E,
    0x6F,
    0xF0,
    0xF1,
    0xF2,
    0xF3,
    0xF4,
    0xF5,
    0xF6,
    0xF7,
    0xF8,
    0xF9,
    0x7A,
    0x7B,
    0x7C,
    0x7D,
    0x7E,
    0x7F,
};
