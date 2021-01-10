#define MAGIC 'X'
#define SIZE1 100

#define CMD_1 _IO(MAGIC,0)
#define CMD_GET _IOW(MAGIC,1,int)
#define CMD_DUMMY _IOR(MAGIC,2,int)

