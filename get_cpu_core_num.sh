# �ܺ��� = ����CPU���� X ÿ������CPU�ĺ��� 
# ���߼�CPU�� = ����CPU���� X ÿ������CPU�ĺ��� X ���߳���

# �鿴����CPU����
cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l

# �鿴ÿ������CPU��core�ĸ���(������)
cat /proc/cpuinfo| grep "cpu cores"| uniq

# �鿴�߼�CPU�ĸ���
cat /proc/cpuinfo| grep "processor"| wc -l
���ƴ���
 �鿴CPU��Ϣ���ͺţ�
cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c

 

�鿴�� ����Ϣ
# cat /proc/meminfo