/* $Id: birthday.c,v 1.7 2000/07/31 07:23:32 edwardc Exp $ */

#include <time.h>
#include <stdio.h>
#include "bbs.h"

main(argc, argv)
char   *argv[];
{
	FILE   *fp, *fout;
	time_t  now;
	int     i, j = 0, add;
	struct userec aman;
	struct tm *tmnow;
	char    buf[256];
	sprintf(buf, "%s/.PASSWDS", BBSHOME);
	if ((fp = fopen(buf, "rb")) == NULL) {
		printf("Can't open record data file.\n");
		return 1;
	}
	sprintf(buf, "%s/0Announce/bbslist/birthday", BBSHOME);
	if ((fout = fopen(buf, "w")) == NULL) {
		printf("Can't write to birthday file.\n");
		return 1;
	}
	now = time(0);
	now += 86400;		/* 直接算明天比較準啦! +1 不準 */

	tmnow = localtime(&now);
	fprintf(fout, "\n%s明日壽星名表\n\n", BBSNAME);
	fprintf(fout, "以下是 %d 月 %d 日的壽星:\n\n", tmnow->tm_mon+1, tmnow->tm_mday);

	for (i = 0;; i++) {
		if (fread(&aman, sizeof(struct userec), 1, fp) <= 0)
			break;
			
		/* 以下的  *名人* 不需要算在內 */
		if (!strcasecmp(aman.userid, "SYSOP") || !strcasecmp(aman.userid, "guest") )
			continue;
			
		if (aman.birthmonth == 0)
			continue;
		if (aman.birthmonth == tmnow->tm_mon + 1 &&
			aman.birthday == tmnow->tm_mday) {
			fprintf(fout, " ** %-15.15s (%s)\n", aman.userid, aman.username);
			j++;
		}
	}
	fprintf(fout, "\n\n總共有 %d 位壽星。\n", j);
	fclose(fout);
	fclose(fp);
}
