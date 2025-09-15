BUILD_DIR := build

.PHONY: all
all: configure build

.PHONY: configure
configure:
	@echo "🔧 Configuring the project..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	  cmake -DCMAKE_BUILD_TYPE=Release \
	        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	        ..

.PHONY: build
build: configure
	@echo "🚧 Building the project..."
	@cmake --build $(BUILD_DIR)

.PHONY: clean
clean:
	@echo "🧹 Cleaning build..."
	@rm -rf $(BUILD_DIR)

.PHONY: rebuild
rebuild: clean all
