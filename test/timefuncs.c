#include "timefuncs.h"

#include <stdlib.h>
#include <time.h>

#include "lvgl/lvgl.h"

static unsigned short unicode(unsigned char ch) {
  unsigned short c = ch;
  return c;
}

static void appendchar(void *des, void *source, int source_len) {
  char *pSource = (char *)source;
  char *pDes = (char *)des;
  while (source_len-- > 0) *((pDes++)) = *pSource++;
}

static int repeatCount(char *str, int strsize) {
  int i = 1;
  if (strsize <= 0) return 0;
  char c = str[0];
  while (i < strsize) {
    if (str[i] == c)
      ++i;
    else
      break;
  }
  return i;
}

static int timeFormatContainsAp(char *str, int strsize) {
  int i = 0;
  while (i < strsize) {
    if (unicode(str[i]) == 'a' || unicode(str[i]) == 'A') return 1;
    ++i;
  }
  return 0;
}

static int timeFormatContainslanguage(char *str, int strsize) {
  int i = 0;
  while (i < strsize) {
    if (unicode(str[i]) == 'e' || unicode(str[i]) == 'E') {
      return 0;
    }
    if (unicode(str[i]) == 'c' || unicode(str[i]) == 'C') {
      return 1;
    }
    ++i;
  }
  return 0;
}

static void getweek(char *str, int *size, int weekindex, int isshort,
                    int language) {
  // 0 english
  // 1 chinese
  if (isshort) {
    if (0 == language) {
      switch (weekindex) {
        case 0: {
          char weekstr[] = "Sun";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 1: {
          char weekstr[] = "Mon";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 2: {
          char weekstr[] = "Tue";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 3: {
          char weekstr[] = "Wed";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 4: {
          char weekstr[] = "Thur";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 5: {
          char weekstr[] = "Fri";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 6: {
          char weekstr[] = "Sat";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
      }
    } else if (1 == language) {
      switch (weekindex) {
        case 0: {
          char weekstr[] = "周日";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 1: {
          char weekstr[] = "周一";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 2: {
          char weekstr[] = "周二";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 3: {
          char weekstr[] = "周三";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 4: {
          char weekstr[] = "周四";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 5: {
          char weekstr[] = "周五";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 6: {
          char weekstr[] = "周六";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
      }
    }
  } else {
    if (0 == language) {
      switch (weekindex) {
        case 0: {
          char weekstr[] = "Sunday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 1: {
          char weekstr[] = "Monday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 2: {
          char weekstr[] = "Tuesday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 3: {
          char weekstr[] = "Wednesday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 4: {
          char weekstr[] = "Thursday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 5: {
          char weekstr[] = "Friday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 6: {
          char weekstr[] = "Saturday";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
      }
    } else if (1 == language) {
      switch (weekindex) {
        case 0: {
          char weekstr[] = "星期天";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 1: {
          char weekstr[] = "星期一";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 2: {
          char weekstr[] = "星期二";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 3: {
          char weekstr[] = "星期三";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 4: {
          char weekstr[] = "星期四";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 5: {
          char weekstr[] = "星期五";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 6: {
          char weekstr[] = "星期六";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
      }
    }
  }
}

static void getmon(char *str, int *size, int monindex, int isshort,
                   int language) {
  // 0 english
  // 1 chinese
  if (isshort) {
    if (0 == language) {
      switch (monindex) {
        case 1: {
          char weekstr[] = "Jan";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 2: {
          char weekstr[] = "Feb";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 3: {
          char weekstr[] = "Mar";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 4: {
          char weekstr[] = "Apr";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 5: {
          char weekstr[] = "May";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 6: {
          char weekstr[] = "Jun";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 7: {
          char weekstr[] = "Jul";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 8: {
          char weekstr[] = "Aug";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 9: {
          char weekstr[] = "Sep";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 10: {
          char weekstr[] = "Oct";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 11: {
          char weekstr[] = "Nov";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 12: {
          char weekstr[] = "Dec";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
      }
    }
  } else {
    if (0 == language) {
      switch (monindex) {
        case 1: {
          char weekstr[] = "January";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 2: {
          char weekstr[] = "February";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 3: {
          char weekstr[] = "March";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 4: {
          char weekstr[] = "April";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 5: {
          char weekstr[] = "May";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 6: {
          char weekstr[] = "June";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 7: {
          char weekstr[] = "July";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 8: {
          char weekstr[] = "August";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 9: {
          char weekstr[] = "September";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 10: {
          char weekstr[] = "October";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 11: {
          char weekstr[] = "November";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
        case 12: {
          char weekstr[] = "December";
          strcpy(str, weekstr);
          *size = sizeof(weekstr);
        } break;
      }
    }
  }

  if (1 == language) {
    switch (monindex) {
      case 1: {
        char weekstr[] = "一月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 2: {
        char weekstr[] = "二月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 3: {
        char weekstr[] = "三月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 4: {
        char weekstr[] = "四月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 5: {
        char weekstr[] = "五月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 6: {
        char weekstr[] = "六月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 7: {
        char weekstr[] = "七月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 8: {
        char weekstr[] = "八月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 9: {
        char weekstr[] = "九月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 10: {
        char weekstr[] = "十月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 11: {
        char weekstr[] = "十一月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
      case 12: {
        char weekstr[] = "十二月";
        strcpy(str, weekstr);
        *size = sizeof(weekstr);
      } break;
    }
  }
}

void dateTimeToString(char *format, int fmsize, char result[]) {
  time_t ts = time(0);
  struct tm *nt = localtime(&ts);

  int year = nt->tm_year + 1900;
  int mon = nt->tm_mon + 1;
  int day = nt->tm_mday;
  int weekindex = nt->tm_wday;
  int hour = nt->tm_hour;
  int min = nt->tm_min;
  int sec = nt->tm_sec;
  int language = timeFormatContainslanguage(format, fmsize);

  int i = 0;
  while (i < fmsize) {
    const char c = format[i];
    int repeat = repeatCount(format + i, fmsize - i);
    int used = 0;
    switch (unicode(c)) {
      case 'y': {
        used = 1;
        if (repeat >= 4)
          repeat = 4;
        else if (repeat >= 2)
          repeat = 2;
        switch (repeat) {
          case 4: {
            char yearstr[5];
            lv_snprintf(yearstr, 5, "%04d", year);
            appendchar(result, yearstr, 4);
            result += 4;
          } break;
          case 2: {
            char yearstr[3];
            int shortyear = year % 100;
            lv_snprintf(yearstr, 3, "%02d", shortyear);
            appendchar(result, yearstr, 2);
            result += 2;
          } break;
          default: {
            repeat = 1;
            memcpy(result, &c, sizeof(c));
            result += 1;
          } break;
        }
      } break;
      case 'M': {
        used = 1;
        repeat = repeat > 4 ? 4 : repeat;
        switch (repeat) {
          case 1: {
            if (mon < 10) {
              char monstr[2];
              lv_snprintf(monstr, 2, "%d", mon);
              appendchar(result, monstr, 1);
              result += 1;
            } else {
              char monstr[3];
              lv_snprintf(monstr, 3, "%d", mon);
              appendchar(result, monstr, 2);
              result += 2;
            }
          } break;
          case 2: {
            char monstr[3];
            lv_snprintf(monstr, 3, "%02d", mon);
            appendchar(result, monstr, 2);
            result += 2;
          } break;
          case 3: {
            char monstr[20];
            int size;
            getmon(monstr, &size, mon, 1, language);
            appendchar(result, monstr, size - 1);
            result += size - 1;
          } break;
          case 4: {
            char monstr[20];
            int size;
            getmon(monstr, &size, mon, 0, language);
            appendchar(result, monstr, size - 1);
            result += size - 1;
          } break;
        }
      } break;
      case 'd': {
        used = 1;
        repeat = repeat > 4 ? 4 : repeat;
        switch (repeat) {
          case 1: {
            if (day < 10) {
              char daystr[2];
              lv_snprintf(daystr, 2, "%d", day);
              appendchar(result, daystr, 1);
              result += 1;
            } else {
              char daystr[3];
              lv_snprintf(daystr, 3, "%d", day);
              appendchar(result, daystr, 2);
              result += 2;
            }
          } break;
          case 2: {
            char daystr[3];
            lv_snprintf(daystr, 3, "%02d", day);
            appendchar(result, daystr, 2);
            result += 2;
          } break;
          case 3: {
            char weekstr[20];
            int size;
            getweek(weekstr, &size, weekindex, 1, language);
            appendchar(result, weekstr, size - 1);
            result += size - 1;
          } break;
          case 4: {
            char weekstr[20];
            int size;
            getweek(weekstr, &size, weekindex, 0, language);
            appendchar(result, weekstr, size - 1);
            result += size - 1;
          } break;
        }
      } break;
      case 'h': {
        used = 1;
        repeat = repeat > 2 ? 2 : repeat;
        int h = hour;
        if (timeFormatContainsAp(format, fmsize)) {
          if (h > 12)
            h -= 12;
          else if (h == 0)
            h = 12;
        }

        switch (repeat) {
          case 1: {
            if (h < 10) {
              char hstr[2];
              lv_snprintf(hstr, 2, "%d", h);
              appendchar(result, hstr, 1);
              result += 1;
            } else {
              char hstr[3];
              lv_snprintf(hstr, 3, "%d", h);
              appendchar(result, hstr, 2);
              result += 2;
            }
          } break;
          case 2: {
            char hstr[3];
            lv_snprintf(hstr, 3, "%02d", h);
            appendchar(result, hstr, 2);
            result += 2;
          } break;
        }
      } break;
      case 'H': {
        used = 1;
        repeat = repeat > 2 ? 2 : repeat;
        int h = hour;
        switch (repeat) {
          case 1: {
            if (h < 10) {
              char hstr[2];
              lv_snprintf(hstr, 2, "%d", h);
              appendchar(result, hstr, 1);
              result += 1;
            } else {
              char hstr[3];
              lv_snprintf(hstr, 3, "%d", h);
              appendchar(result, hstr, 2);
              result += 2;
            }
          } break;
          case 2: {
            char hstr[3];
            lv_snprintf(hstr, 3, "%02d", h);
            appendchar(result, hstr, 2);
            result += 2;
          } break;
        }
      } break;
      case 'm': {
        used = 1;
        repeat = repeat > 2 ? 2 : repeat;
        switch (repeat) {
          case 1: {
            if (min < 10) {
              char minstr[2];
              lv_snprintf(minstr, 2, "%d", min);
              appendchar(result, minstr, 1);
              result += 1;
            } else {
              char minstr[3];
              lv_snprintf(minstr, 3, "%d", min);
              appendchar(result, minstr, 2);
              result += 2;
            }
          } break;
          case 2: {
            char minstr[3];
            lv_snprintf(minstr, 3, "%02d", min);
            appendchar(result, minstr, 2);
            result += 2;
          } break;
        }
      } break;
      case 's': {
        used = 1;
        repeat = repeat > 2 ? 2 : repeat;
        switch (repeat) {
          case 1: {
            if (sec < 10) {
              char secstr[2];
              lv_snprintf(secstr, 2, "%d", sec);
              appendchar(result, secstr, 1);
              result += 1;
            } else {
              char secstr[3];
              lv_snprintf(secstr, 3, "%d", sec);
              appendchar(result, secstr, 2);
              result += 2;
            }
          } break;
          case 2: {
            char secstr[3];
            lv_snprintf(secstr, 3, "%02d", sec);
            appendchar(result, secstr, 2);
            result += 2;
          } break;
        }
      } break;
      case 'a': {
        used = 1;
        if (i + 1 < fmsize && unicode(format[i + 1]) == 'p') {
          repeat = 2;
        } else {
          repeat = 1;
        }
        if (hour < 12) {
          if (0 == language) {
            char amstr[] = "am";
            appendchar(result, amstr, sizeof(amstr) - 1);
            result += sizeof(amstr) - 1;
          } else if (1 == language) {
            char amstr[] = "上午";
            appendchar(result, amstr, sizeof(amstr) - 1);
            result += sizeof(amstr) - 1;
          }
        } else {
          if (0 == language) {
            char pmstr[] = "pm";
            appendchar(result, pmstr, sizeof(pmstr) - 1);
            result += sizeof(pmstr) - 1;
          } else if (1 == language) {
            char pmstr[] = "下午";
            appendchar(result, pmstr, sizeof(pmstr) - 1);
            result += sizeof(pmstr) - 1;
          }
        }
      } break;
      case 'A': {
        used = 1;
        if (i + 1 < fmsize && unicode(format[i + 1]) == 'P') {
          repeat = 2;
        } else {
          repeat = 1;
        }
        if (hour < 12) {
          char amstr[] = "AM";
          appendchar(result, amstr, 2);
          result += 2;
        } else {
          char pmstr[] = "PM";
          appendchar(result, pmstr, 2);
          result += 2;
        }
      } break;
      case 'c': {
        used = 1;
        if (i + 1 < fmsize && unicode(format[i + 1]) == 'h') {
          repeat = 2;
        } else {
          repeat = 1;
        }
      } break;
      case 'C': {
        used = 1;
        if (i + 1 < fmsize && unicode(format[i + 1]) == 'H') {
          repeat = 2;
        } else {
          repeat = 1;
        }
      } break;
      case 'e': {
        used = 1;
        if (i + 1 < fmsize && unicode(format[i + 1]) == 'n') {
          repeat = 2;
        } else {
          repeat = 1;
        }
      } break;
      case 'E': {
        used = 1;
        if (i + 1 < fmsize && unicode(format[i + 1]) == 'N') {
          repeat = 2;
        } else {
          repeat = 1;
        }
      } break;
    }
    if (!used) {
      for (int j = 0; j < repeat; ++j) {
        (*result) = c;
        ++result;
      }
    }
    i += repeat;
  }
}
