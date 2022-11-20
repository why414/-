#include "serial_port.h"
//����ṹ��---����ṹ��
static union
{
    char Receive_Val[24];
    float Act_val[6];
}uart_posture;

int ret;
int fd;

const char name[] = "/dev/ttyUSB0";//��ΪUART0_Init()�����һ��������const ���Զ���һ��const����
char head[2] = { 0x0d,0x0a };
char tail[2] = { 0x0a,0x0d };
int Init_ = false;
Seral_Send_ReaD serial_port;//USB send data


void send_data(float Data, float depth)
{
    if (Init_ == false) { serial_port.UART0_Init(115200, 0, 8, 1, 0, name); Init_ = true; }
    data send;
    send.data_need[0] = Data;
    send.data_need[1] = depth;
    //cout<<Data<<endl;
    serial_port.uart_write(head, 2);
    serial_port.uart_write(send.data_send, 8);
    serial_port.uart_write(tail, 2);
}

/*
 * ��ȫ��д����
 */

ssize_t Seral_Send_ReaD::safe_write(const char* vptr, size_t n)
{
    size_t  nleft;
    ssize_t nwritten;
    const char* ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return(n);
}

ssize_t Seral_Send_ReaD::safe_read(char* vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char* ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)//���ź��ж�
                nread = 0;
            else
                return -1;
        }
        else
            if (nread == 0)
                break;
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);
}
/*******************************************************************
* ���ƣ�                  UART0_Open
* ���ܣ�                �򿪴��ڲ����ش����豸�ļ�����
* ��ڲ�����        fd    :�ļ�������     pathname :���ں�(ttyS0,ttyS1,ttyS2)
* ���ڲ�����        ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int Seral_Send_ReaD::uart_open(const char* pathname)
{
    assert(pathname);

    /*�򿪴���*/
    fd = open(pathname, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == FALSE)
    {
        perror("Open UART failed!");
        return FALSE;
    }

    /*������ڷ�������־*/
    if (fcntl(fd, F_SETFL, 0) < 0)
    {
        fprintf(stderr, "fcntl failed!\n");
        return FALSE;
    }
    return fd;
}

/*******************************************************************
* ���ƣ�                UART0_Set
* ���ܣ�                ���ô�������λ��ֹͣλ��Ч��λ
* ��ڲ�����					   fd        �����ļ�������
*                              baude     ������
*                              c_flow    ����������
*							   bits      ����λ   ȡֵΪ 7 ����8
*							   parity    Ч������ ȡֵΪN,E,O,,S
*                              stop      ֹͣλ   ȡֵΪ 1 ����2
*
*���ڲ�����          ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int Seral_Send_ReaD::uart_set(int baude, int c_flow, int bits, char parity, int stop)
{
    struct termios options;

    /*��ȡ�ն�����*/
    if (tcgetattr(fd, &options) < 0)
    {
        perror("tcgetattr error");
        return FALSE;
    }


    /*����������������ʣ����߱���һ��*/
    switch (baude)
    {
    case 4800:
        cfsetispeed(&options, B4800);
        cfsetospeed(&options, B4800);
        break;
    case 9600:
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);
        break;
    case 115200:
        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);
        break;
    case 19200:
        cfsetispeed(&options, B19200);
        cfsetospeed(&options, B19200);
        break;
    case 38400:
        cfsetispeed(&options, B38400);
        cfsetospeed(&options, B38400);
        break;
    default:
        fprintf(stderr, "Unkown baude!\n");
        return FALSE;
    }

    /*���ÿ���ģʽ*/
    options.c_cflag |= CLOCAL;//��֤����ռ�ô���
    options.c_cflag |= CREAD;//��֤������ԴӴ����ж�ȡ����

    /*��������������*/
    switch (c_flow)
    {
    case 0://������������
        options.c_cflag &= ~CRTSCTS;
        break;
    case 1://����Ӳ��������
        options.c_cflag |= CRTSCTS;
        break;
    case 2://��������������
        options.c_cflag |= IXON | IXOFF | IXANY;
        break;
    default:
        fprintf(stderr, "Unkown c_flow!\n");
        return -1;
    }

    /*��������λ*/
    options.c_cflag &= ~CSIZE;//����������־λ
    switch (bits)
    {
    case 5:
        options.c_cflag |= CS5;
        break;
    case 6:
        options.c_cflag |= CS6;
        break;
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Unkown bits!\n");
        return FALSE;
    }

    /*����У��λ*/
    switch (parity)
    {
        /*����żУ��λ*/
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;//PARENB��������żλ��ִ����żУ��
        //options.c_cflag &= ~INPCK;//INPCK��ʹ��żУ��������//lzz
        break;
        /*��Ϊ�ո�,��ֹͣλΪ2λ*/
    case 's':
    case 'S':
        options.c_cflag &= ~PARENB;//PARENB��������żλ��ִ����żУ��
        options.c_cflag &= ~CSTOPB;//CSTOPB��ʹ����λֹͣλ
        break;
        /*������У��*/
    case 'o':
    case 'O':
        options.c_cflag |= PARENB;//PARENB��������żλ��ִ����żУ��
        options.c_cflag |= PARODD;//PARODD����������Ϊ��У��,����ΪżУ��
        options.c_cflag |= INPCK;//INPCK��ʹ��żУ��������
        options.c_cflag |= ISTRIP;//ISTRIP������������Ч�������ֱ�����7���ֽڣ�������ȫ��8λ
        break;
        /*����żУ��*/
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;//PARENB��������żλ��ִ����żУ��
        options.c_cflag &= ~PARODD;//PARODD����������Ϊ��У��,����ΪżУ��
        options.c_cflag |= INPCK;//INPCK��ʹ��żУ��������
        options.c_cflag |= ISTRIP;//ISTRIP������������Ч�������ֱ�����7���ֽڣ�������ȫ��8λ
        break;
    default:

        options.c_cflag &= ~PARENB;//PARENB��������żλ��ִ����żУ��
        options.c_cflag &= ~INPCK;//INPCK��ʹ��żУ��������

        fprintf(stderr, "Unkown parity!\n");
        //return -1;
    }

    /*����ֹͣλ*/
    switch (stop)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;//CSTOPB��ʹ����λֹͣλ
        break;
    case 2:
        options.c_cflag |= CSTOPB;//CSTOPB��ʹ����λֹͣλ
        break;
    default:
        fprintf(stderr, "Unkown stop!\n");
        return -1;
    }

    /*�������ģʽΪԭʼ���*/
    options.c_oflag &= ~OPOST;//OPOST���������򰴶���������������������c_oflagʧЧ
    /*���ñ���ģʽΪԭʼģʽ*/
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //options.c_lflag &= ~(ISIG | ICANON); 
    /*
     *ICANON�������淶ģʽ�������봦��
     *ECHO�����������ַ��ı��ػ���
     *ECHOE���ڽ���EPASEʱִ��Backspace,Space,Backspace���
     *ISIG�������ź�
     */


     /*���õȴ�ʱ�����С�����ַ�*/
    options.c_cc[VTIME] = 0;//������select������ // ��ȡһ���ַ��ȴ�0*(1/10)s
    options.c_cc[VMIN] = 1;//���ٶ�ȡһ���ַ�

    /*����������������ֻ�������ݣ����ǲ����ж�����*/
    tcflush(fd, TCIFLUSH);

    /*��������*/
    if (tcsetattr(fd, TCSANOW, &options) < 0)
    {
        perror("tcsetattr failed");
        return FALSE;
    }

    return TRUE;
}

int Seral_Send_ReaD::uart_read(char* r_buf, size_t len)
{
    ssize_t cnt = 0;
    fd_set rfds;
    struct timeval time;

    /*���ļ����������������������*/
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /*���ó�ʱΪ15s*/
    time.tv_sec = 15;
    time.tv_usec = 0;

    /*ʵ�ִ��ڵĶ�·I/O*/
    ret = select(fd + 1, &rfds, NULL, NULL, &time);
    switch (ret)
    {
    case -1:
        fprintf(stderr, "select error!\n");
        return -1;
    case 0:
        fprintf(stderr, "time over!\n");
        return -1;
    default:
        cnt = safe_read(r_buf, len);
        if (cnt == -1)
        {
            fprintf(stderr, "read error!\n");
            return FALSE;
        }
        return cnt;
    }
}

int Seral_Send_ReaD::uart_write(const char* w_buf, size_t len)
{
    ssize_t cnt = 0;

    cnt = safe_write(w_buf, len);
    if (cnt == -1)
    {
        fprintf(stderr, "write error!\n");
        return FALSE;
    }

    return cnt;
}
int Seral_Send_ReaD::UART0_Init(int speed, int flow_ctrl, int databits, int stopbits, int parity, const char* pathname)
{
    int err;
    //���ô�������֡��ʽ  
    fd = uart_open(pathname);
    if (Seral_Send_ReaD::uart_set(speed, flow_ctrl, databits, 'N', stopbits) == FALSE)

    {
        return FALSE;
    }
    else
    {
        return  TRUE;
    }
}


int  Seral_Send_ReaD::uart_send_two(float data1, float data2)
{
    uart_posture.Act_val[0] = data1;
    uart_posture.Act_val[1] = data2;

    char sendData[8];
    for (size_t i = 0; i < 8; i++) {
        sendData[i] = uart_posture.Receive_Val[i];
    }
    uart_write(sendData, 8);
    return 0;
}

int Seral_Send_ReaD::uart_close()
{
    assert(fd);
    close(fd);

    /*������������Щ��������*/

    return TRUE;
}