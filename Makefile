# (1)コンパイラ
CC  = g++
# (2)コンパイルオプション
CFLAGS = -Wall -O2 -std=c++11
# (3)実行ファイル名
TARGET = main
# (4)コンパイル対象のソースコード
SRCS = edge.cpp vertex.cpp face.cpp dice.cpp main.cpp
# (5)オブジェクトファイル名
OBJS = $(SRCS:.cpp=.o)
# (6)インクルードファイルのあるディレクトリパス
INCDIR  =
# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  =
# (8)追加するライブラリファイル
LIBS    =

# (9)ターゲットファイル生成
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)

# (10)オブジェクトファイル生成
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)
# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	-rm -f $(OBJS) $(TARGET) *.d
