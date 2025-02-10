

start: run
	@echo "Start suka start.."
	echo $(PATH)
	go version
	go run .

# .PHONY: start
# run:
# 	@echo "Run suka run..."
# 	echo $(PATH)
#	go run 

# valgrind: rebuild
# 	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leaks_log.txt $(OUT_TEST)
# 	echo --- Valgrind summary --- && cat leaks_log.txt | grep 'total heap usage' && cat leaks_log.txt | grep 'ERROR SUMMARY' 


# Собираем docker image с тегом 'v1' на основании dockerfile и создаем контейнер
docker_build:
	docker build -t xgo_vault:v1 .
#	docker run --name good_morning -it tetris_vault:v1 make start
#	docker run --name good_morning -it tetris_vault:v1 /bin/bash -c "./tetris" -v /tmp/.X11-unix:/tmp/.X11-unix
	docker run -e DISPLAY=unix$(DISPLAY) -v /tmp/.X11-unix:/tmp/.X11-unix --name good_evening -it xgo_vault:v1 make start
#	xhost +local:docker
	xhost +local:
#	docker run -v /tmp/.X11-unix:/tmp/.X11-unix --name good_morning -it tetris_vault:v1 make start
	docker rm good_evening
	docker image rm xgo_vault:v1