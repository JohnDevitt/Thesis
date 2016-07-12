      PROGRAM MATMULPROG

C     Copyright (C) 2000-2010 by Grigori G.Fursin
C
C     http://fursin.net/research
C
C     UNIDAPT Group
C     http://unidapt.org

      IMPLICIT REAL (A-F,X)

      PARAMETER (N=128)
      COMMON A(N,N), B(N,N), C(N,N)

      WRITE (*,*) 'MATMUL'
      WRITE (*,*) ' '

      WRITE (*,*) 'Preparing ...'
      CALL PREP

      WRITE (*,*) 'Calculating ...'
      DO I=1, 1000
       CALL MATMUL
      END DO

      WRITE (*,*) ' '
      WRITE (*,*) 'X=', A(10,10)

      WRITE (*,*) ' '
      WRITE (*,*) 'End'

      END

      SUBROUTINE PREP

      IMPLICIT REAL (A-F,X)

      PARAMETER (N=128)
      COMMON A(N,N), B(N,N), C(N,N)

      DO J=1, N
       DO I=1, N
        D=I+J
        A(I,J)=SIN(D+1)
        B(I,J)=SIN(D+2)
        C(I,J)=SIN(D+3)
       END DO
      END DO

      END

      SUBROUTINE MATMUL

      IMPLICIT REAL (A-F,X)

      PARAMETER (N=128)
      COMMON A(N,N), B(N,N), C(N,N)

      DO I=1, N
C      A(I,J)=0
       DO J=1, N
        DO K=1, N
         A(I,J)=A(I,J)+B(I,K)*C(K,J)
        END DO
       END DO
      END DO

      END
