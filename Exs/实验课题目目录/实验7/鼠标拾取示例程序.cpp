#include "glut.h" 
int board[3][3];   //�洢�����������ɫ
#define BUFSIZE 512
//��������¼:
//hitsΪ�����ĵ����������buffer�д洢�����¼��ÿ�������¼���ĸ���Ŀ���
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint ii, jj, names, *ptr;
	ptr = (GLuint *)buffer;

	for (i = 0; i<hits; i++)    //����ÿһ�������¼
	{
		//ĳһ�������¼��˵�����ĸ���Ŀ��ɣ�
		//��1�����������ʱ�����ֶ�ջ�е���������
		//��2������һ�������¼֮�����Ӿ����ཻ�����ж������С����󴰿�����zֵ
		//��3�����������ʱ�����ƶ�ջ�����ݣ�����ײ���Ԫ�ؿ�ʼ
		names = *ptr;      //������ֶ�ջ�е���������
		ptr += 3;               //����ǰ������¼
		for (j = 0; j<names; j++) //��ʼ�������ֶ�ջ�е����ݣ���ȡ��ѡ�еķ����index
		{
			//��Ӧ�ڻ��Ʒ���ʱ��ѹ�����ֶ�ջ�е�����
			if (j == 0)        //x�����ϵ�index
				ii = *ptr;
			else if (j == 1)  //y�����ϵ�index
				jj = *ptr;
			ptr++;
		}
	}
	board[ii][jj] = (board[ii][jj] + 1) % 3;   //�ı䱻ѡ�з������ɫ
}

//�������з��飬������GL_RENDER��GL_SELECT����ģʽ
void drawSquares(GLenum mode)
{
	GLuint i, j;
	for (i = 0; i<3; i++)
	{
		if (mode == GL_SELECT)       //�������ѡ��ģʽ�£������ֶ�ջ����Ԫ�ػ���x�����ϵ�����
			glLoadName(i);

		for (j = 0; j<3; j++)
		{
			if (mode == GL_SELECT)    //��y�����ϵ�����ѹ�����ֶ�ջ
				glPushName(j);

			//���Ʒ��飬��GL_SELECTģʽ�£�ĳһ������ᱻѡ�У���ˣ������һ�������¼
			//�õ������¼ʱ�����ֶ�ջ�����������ƣ��ֱ���i��j��ֵ��Ҳ���Ǳ�ѡ�з��������
			glColor3f((GLfloat)i / 3.0, (GLfloat)j / 3.0, (GLfloat)board[i][j] / 3.0);
			glRecti(i, j, i + 1, j + 1);

			if (mode == GL_SELECT) //��������
				glPopName();
		}
	}
}

//���������������ʱ������ѡ��ģʽ��ʼ���ƣ�����֮�󣬴�������¼
void pickSquares(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];   //�洢�����¼
	GLint hits;                                 //�����¼�ĸ���
	GLint viewport[4];                    //�ӿ�

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	glGetIntegerv(GL_VIEWPORT, viewport);  //����ӿ�
	glSelectBuffer(BUFSIZE, selectBuf);    //ָ���洢�����¼������
	glRenderMode(GL_SELECT);          //����ѡ��ģʽ

	glInitNames();           //��ʼ�����ֶ�ջ��ѹ���ʼԪ��
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//������ѡ������ѡ���������������(x,viewport[3]-y)����С��(5,5)
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
	//����ͶӰ����
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);
	//��ѡ��ģʽ�»��Ʒ���
	drawSquares(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();        //���ƽ���

	//��������¼
	hits = glRenderMode(GL_RENDER); //��ȡ��¼�µĵ���ĸ���
	processHits(hits, selectBuf);           //��������¼selectBuf
	glutPostRedisplay();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	for (int i = 0; i <3; i++)            //��ʼ��9���������ɫ
		for (int j = 0; j<3; j++)
			board[i][j] = 0;
}
void display()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSquares(GL_RENDER);  //��������
	glFlush();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("pick");
	init();
	glutMouseFunc(pickSquares);   //�������ʱ������pickSquares������ѡ��ģʽ���л���
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);      //displayֻ��ɻ����Ļ���
	glutMainLoop();
	return 0;
}