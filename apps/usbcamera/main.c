//加载标准库文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//加载Linux标准的文件IO操作文件
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

//加载Linux图像标准文件
#include <linux/videodev2.h>

static int camera_open(void)
{
	int fd_v4l = -1;

	fd_v4l = open("/dev/video0", O_RDWR);
	if(fd_v4l < 0){
		printf("lml Can't open /dev/video0 ...\n");	
	}

	return fd_v4l;
}

static void camera_close(int fd)
{
	close(fd);
}

int main(int argc, char *argv[])
{
	int fd = 0;

	struct v4l2_capability cap;//设备的属性
	struct v4l2_fmtdesc fmtdesc;//显示设备支持的格式


	//打开cam设备
	fd = camera_open();
	if(fd)
		printf("lml camera_open success ... fd = %d \n", fd);

	//获取摄像头的驱动参数
	ioctl(fd, VIDIOC_QUERYCAP, &cap);

	printf("cap driver name is %s ...\n", cap.driver);
	printf("cap card name is %s ...\n", cap.card);
	printf("cap bus info is %s ...\n", cap.bus_info);
	printf("cap version is 0x%x ...\n", cap.version);
	printf("cap capabilities is 0x%x ...\n", cap.capabilities);
	printf("cap device_caps is %d ...\n", cap.device_caps);

	//获取摄像头支持的格式
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	while(ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1){
		printf("fmtdesc Format number %d...\n", fmtdesc.index);
		printf("fmtdesc type %d...\n", fmtdesc.type);
		printf("fmtdesc Format descriptions %s...\n", fmtdesc.description);
		printf("fmtdesc pixel Format  %d...\n", fmtdesc.pixelformat);
		fmtdesc.index++;
	}

	//关闭摄像头
	camera_close(fd);

	return 0;
}
