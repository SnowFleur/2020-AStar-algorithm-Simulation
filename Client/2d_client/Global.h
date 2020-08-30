#pragma once
#include<memory>
#include<vector>
#include<utility>

class  CTexture;

using Position = std::pair<int,int>; //x, y
using VectorInObject = std::vector<Position>;                           //using vector in Position
using SharedPtrTexture = std::shared_ptr<CTexture>;                     //using shared ptr CTexture
using SharedPtrTextures = std::vector<std::vector< SharedPtrTexture>>;  //using shared ptr in vector


constexpr int MAX_WORLD_X = 14;
constexpr int MAX_WORLD_Y = 10;
constexpr int RESOURCE_HEIGHT = 70;
constexpr int RESOURCE_WIDTH = 770;




/*
2. �˰��� ����
(1) ���������� �������(Openlist)�� �ִ´�.
(2) ������Ͽ� �ִ� ��� �� 1���� ���� ���� ���� �ֺ���带 Ž���Ѵ�.

( ���Լ� F= G+H �� ��� & �θ��带 ���, ��ֹ��� ��������� �����Ѵ�.)
F= G+H ����
G : ���۳��N0���� ������N������ �ִܰ���� ��. (����,���� +1, �밢���� ��� +1.41)
H : ������N���� ��ǥ�������� (���Ǿ���)�ִܰ���� ��. (�޸���ƽ���)
F :  ���۳��N0���� ��ǥ������ ������N�� ���� �� �� �ִ� ��� ������ ��� ��  �ִܰ���� ���� �ȴ�.


(3) 2�ܰ迡�� �� ��带 �������(Closelist)�� �����Ѵ�

(4) 2�ܰ迡�� Ž���� ������ ���� ���(�켱���� queue)�� �����Ѵ�
(�켱���� ť�� ���� ���� ������ ������� �ڵ����ĵǴ� Ư���� ������ ������ 
���� ���� F����� ���� ��带 ã�� �� �ִ�.)

(5) ���� ��� �� ���� �� ��带 ���� �� ��带 ���� ��Ͽ� �߰��Ѵ�.

(6) 5�ܰ迡�� �� ����� �������� �ֿ� ��带 Ž���Ѵ�.

(��ֹ��� �����������, ��ǥ��尡 �ִ��� ����)
(������ ���: 1. ���� ��ǥ��尡 �ִٸ� �θ��带 �����Ͽ� �������� ���ÿ� ����
2. ��������� ����ְ� �� ��� ��ǥ��带 ã�µ� ������ ��, ���� ����.)

(7) ������Ͽ� �������� �ʴ� ���� ������Ͽ� �߰��ϰ�

�ߺ��Ǵ� ���� G���� ���κ��Ͽ� �� ���� ���� ���� ������� ��ü�Ѵ�.

(8) 5�ܰ���� �ݺ�

 */