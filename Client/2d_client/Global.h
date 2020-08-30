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
2. 알고리즘 순서
(1) 시작지점을 열린목록(Openlist)에 넣는다.
(2) 열린목록에 있는 노드 중 1개를 빼서 여덟 방향 주변노드를 탐색한다.

( 평가함수 F= G+H 를 계산 & 부모노드를 명시, 장애물과 닫힌목록은 제외한다.)
F= G+H 설명
G : 시작노드N0에서 현재노드N까지의 최단경로의 값. (수직,수평 +1, 대각선일 경우 +1.41)
H : 현재노드N에서 목표노드까지의 (조건없는)최단경로의 값. (휴리스틱요소)
F :  시작노드N0에서 목표노드까지 현재노드N을 통해 갈 수 있는 모든 가능한 경로 중  최단경로의 값이 된다.


(3) 2단계에서 뺀 노드를 닫힌목록(Closelist)에 삽입한다

(4) 2단계에서 탐색한 노드들을 열린 목록(우선순위 queue)에 삽입한다
(우선순위 큐는 가장 작은 값부터 순서대로 자동정렬되는 특성을 가지기 때문에 
가장 낮은 F비용을 가진 노드를 찾을 수 있다.)

(5) 열린 목록 중 가장 앞 노드를 빼고 그 노드를 닫힌 목록에 추가한다.

(6) 5단계에서 뺀 노드의 여덟방향 주요 노드를 탐색한다.

(장애물과 닫힌목록제외, 목표노드가 있는지 조사)
(끝내는 경우: 1. 만일 목표노드가 있다면 부모노드를 추적하여 역순으로 스택에 삽입
2. 열린목록이 비어있게 될 경우 목표노드를 찾는데 실패한 것, 길이 없음.)

(7) 열린목록에 존재하지 않는 노드는 열린목록에 추가하고

중복되는 노드는 G값을 서로비교하여 더 작은 값을 열린 목록으로 교체한다.

(8) 5단계부터 반복

 */