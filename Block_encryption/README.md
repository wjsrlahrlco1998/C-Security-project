# 블록 암호화

*암호화 알고리즘 테스트 및 클라이언트 & 서버를 구축하여 암호화된 이미지, 텍스트를 주고 받고 인증 프로토콜의 과정을 수행한다*



### * 라이브러리

``C-Openssl 라이브러리``



## 1. Text 암호화



### 1) DES 암호화 & 복호화 결과

- 평문

![image-20220630224704917](README.assets/image-20220630224704917.png)

- 암호문

![image-20220630224715126](README.assets/image-20220630224715126.png)

- 복호문

![image-20220630224642004](README.assets/image-20220630224642004.png)



### 2) 3-DES 암호문 & 복호문

- 평문은 DES와 동일하다.

- 암호문

![image-20220630224753056](README.assets/image-20220630224753056.png)

- 복호문

![image-20220630224802016](README.assets/image-20220630224802016.png)



### 3) AES 암호문 & 복호문

- 평문은 DES와 동일
- 암호문

![image-20220630224843618](README.assets/image-20220630224843618.png)

- 복호문

![image-20220630224852129](README.assets/image-20220630224852129.png)



### 4) 클라이언트(암호화) & 서버(복호화) 파일 전송

*DES, 3-DES, AES 3가지 모드 중 AES의 경우만 보여줌*

- Server

![image-20220630225138510](README.assets/image-20220630225138510.png)

- 평문

![image-20220630225159479](README.assets/image-20220630225159479.png)

- 암호문

![image-20220630225211760](README.assets/image-20220630225211760.png)

- Client

![image-20220630225222294](README.assets/image-20220630225222294.png)

- 암호문(전송받음)

![image-20220630225235803](README.assets/image-20220630225235803.png)

- 복호문

![image-20220630225245386](README.assets/image-20220630225245386.png)



## 2. 이미지(.bmp) 암호화

 

- 원본 이미지

![image-20220630225351457](README.assets/image-20220630225351457.png)

- DES ECB 모드 암호화

![image-20220630225409122](README.assets/image-20220630225409122.png)

- 3-DES ECB 모드 암호화

![image-20220630225433117](README.assets/image-20220630225433117.png)

- AES ECB 모드 암호화

![image-20220630225450484](README.assets/image-20220630225450484.png)

- DES CBC 모드 암호화

![image-20220630225510192](README.assets/image-20220630225510192.png)

- 3-DES CBC모드 암호화

![image-20220630225526812](README.assets/image-20220630225526812.png)

- AES CBC모드 암호화

![image-20220630225543757](README.assets/image-20220630225543757.png)



