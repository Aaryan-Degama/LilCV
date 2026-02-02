#include "image_import.hpp"

// In image_import.cpp
Image::Image(int width, int height)
    : m_width(width), m_height(height), pixels(width * height)
{
}

int Image::width() const{
    return m_width;
}

int Image::height() const{
    return m_height;
}

Pixel &Image::at(int x, int y){
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        throw std::out_of_range("Image::at() index out of range");
    return pixels[y * m_width + x];
}

const Pixel &Image::at(int x, int y) const{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        throw std::out_of_range("Image::at() index out of range");
    return pixels[y * m_width + x];
}

// This functions loads the data of the image
Image loadPPM(const std::string &ifile){
    std::ifstream limg(ifile); // load image == limg
    if (!limg.is_open()){ 
        // Checks if the file was opened
        throw std::runtime_error("Failed to open loading image");
    }
    std::string type;
    limg >> type;

    if (type != "P3") { 
        // checks if the type is correct
        throw std::runtime_error("Only P3 PPM supported");
    }

    int width, height;
    int maxRGB;

    if (limg >> width >> height >> maxRGB) {
        if (maxRGB != 255) { 
            // Checks if the max RGB is correct or not
            throw std::runtime_error("Only max RGB = 255 are supported");
        }
        Image img(width, height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Pixel &p = img.at(x, y);
                limg >> p.r >> p.g >> p.b;
            }
        }
        return img;
    }
    else {
        throw std::runtime_error("File was'nt able to be extracted");
    }
}

void savePPM(const Image &img, const std::string &ofile) {
    std::ofstream simg(ofile); // save image == simg
    if (!simg.is_open())
        throw std::runtime_error("Failed to open saving image");

    simg << "P3\n"
         << img.width() << " " << img.height() << "\n255\n";

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            const Pixel &p = img.at(x, y);
            simg << static_cast<int>(p.r)
                 << " " << static_cast<int>(p.g)
                 << " " << static_cast<int>(p.b) << " ";
        }
        simg << '\n';
    }
}

const Image bw(const Image &img) {

    Image output(img.width(), img.height());

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++){
            const Pixel &real_pixel = img.at(x, y);
            Pixel &new_pixel = output.at(x, y);
            float a{(real_pixel.r + real_pixel.g + real_pixel.b) / 3};
            new_pixel.r = a;
            new_pixel.g = a;
            new_pixel.b = a;
        }
    }
    return output;
}

const Image blur(const Image &img, double intensity) {

    intensity = std::max(intensity, 0.5);

    Image output(img.width(), img.height());
    int size = static_cast<int>(std::ceil(intensity * 6)) | 1;
    size = std::max(size, 3);

    if (size % 2 == 0)
        size++;
    if (size < 3)
        size = 3;
    int half = size / 2;

    std::vector<float> kernel1D(size);
    float sum = 0.0f;
    for (int i = -half; i <= half; i++) {
        float val = exp(-(i * i) / (2.0f * intensity * intensity));
        kernel1D[i + half] = val;
        sum += val;
    }
    for (float &w : kernel1D)
        w /= sum;

    Image temp(img.width(), img.height());

    // Horizontal Pass
    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            float r = 0, g = 0, b = 0; // Accumulators
            for (int i = -half; i <= half; i++) {
                int ix = std::clamp(x + i, 0, img.width() - 1);
                const Pixel &p = img.at(ix, y);
                r += p.r * kernel1D[i + half];
                g += p.g * kernel1D[i + half];
                b += p.b * kernel1D[i + half];
            }
            Pixel &outP = temp.at(x, y);
            outP.r = r;
            outP.g = g;
            outP.b = b;
        }
    }

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            float r = 0, g = 0, b = 0; // Accumulators
            for (int i = -half; i <= half; i++) {
                int iy = std::clamp(y + i, 0, img.height() - 1); // Fixed: height
                const Pixel &p = temp.at(x, iy);                 // Fixed: read from temp
                r += p.r * kernel1D[i + half];
                g += p.g * kernel1D[i + half];
                b += p.b * kernel1D[i + half];
            }
            Pixel &outP = output.at(x, y);
            outP.r = r;
            outP.g = g;
            outP.b = b;
        }
    }

    return output;
}

const Image negative(const Image& img) {
    Image output(img.width(), img.height());

    for(int y=0;y<img.height();y++) {
        for(int x=0;x<img.width();x++) {
            const Pixel& p = img.at(x,y);
            Pixel& o = output.at(x,y);

            o.r = 255.f - p.r;
            o.g = 255.f - p.g;
            o.b = 255.f - p.b;
        }
    }
    return output;
}

const Image resize(const Image& img, int newW, int newH){
    Image output(newW, newH);

    int W = img.width();
    int H = img.height();

    for(int y=0;y<newH;y++){
        for(int x=0;x<newW;x++){
            int sx = x * W / newW;
            int sy = y * H / newH;

            const Pixel& p = img.at(sx, sy);
            Pixel& o = output.at(x,y);

            o.r = p.r;
            o.g = p.g;
            o.b = p.b;
        }
    }
    return output;
}

Image computeDepthMap(const Image &img) {
    static Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "midas");
    static Ort::Session *session = nullptr;

    if (!session) {
        Ort::SessionOptions opts;
        opts.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
        session = new Ort::Session(env, "midas_small.onnx", opts);
    }

    const int IN = 256;
    const int H = img.height();
    const int W = img.width();

    std::vector<float> input(1 * 3 * IN * IN);

    for (int y = 0; y < IN; y++) {
        for (int x = 0; x < IN; x++) {
            int sx = x * W / IN;
            int sy = y * H / IN;
            const Pixel &p = img.at(sx, sy);

            int i = y * IN + x;
            input[i + 0 * IN * IN] = p.r / 255.f;
            input[i + 1 * IN * IN] = p.g / 255.f;
            input[i + 2 * IN * IN] = p.b / 255.f;
        }
    }

    std::array<int64_t, 4> shape{1, 3, IN, IN};
    Ort::MemoryInfo mem = Ort::MemoryInfo::CreateCpu(
        OrtDeviceAllocator, OrtMemTypeCPU);

    Ort::Value tin = Ort::Value::CreateTensor<float>(
        mem, input.data(), input.size(), shape.data(), 4);

    const char *in_names[] = {"input"};
    const char *out_names[] = {"depth"};

    auto out = session->Run({}, in_names, &tin, 1, out_names, 1);
    float *d = out[0].GetTensorMutableData<float>();

    float mn = d[0], mx = d[0];
    for (int i = 0; i < IN * IN; i++) {
        mn = std::min(mn, d[i]);
        mx = std::max(mx, d[i]);
    }

    Image depth(W, H);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int dx = x * IN / W;
            int dy = y * IN / H;
            float v = (d[dy * IN + dx] - mn) / (mx - mn + 1e-6f);

            Pixel &p = depth.at(x, y);
            p.r = p.g = p.b = v;
        }
    }
    return depth;
}

const Image depthBlur(const Image &img, double intensity) {
    // --- smooth depth slightly to remove noise ---
    Image depth = blur(computeDepthMap(img), 1.2);

    Image output(img.width(), img.height());

    const int LEVELS = 12;
    std::vector<Image> blurred;
    blurred.reserve(LEVELS);

    for (int i = 0; i < LEVELS; i++) {
        double t = static_cast<double>(i) / (LEVELS - 1);
        blurred.push_back(blur(img, intensity * t));
    }

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {

            double d = depth.at(x, y).r;
            double inv = 1.0 - d;

            // ---- edge-aware depth guard ----
            if (x > 0 && y > 0) {
                double dn = depth.at(x - 1, y).r;
                if (std::abs(d - dn) > 0.15) {
                    inv = 0.0; // force sharp edge
                }
            }

            int idx = std::clamp(
                static_cast<int>(inv * (LEVELS - 1)),
                0, LEVELS - 1);

            const Pixel &p = blurred[idx].at(x, y);
            Pixel &out = output.at(x, y);

            out.r = p.r;
            out.g = p.g;
            out.b = p.b;
        }
    }

    return output;
}

const Image depthImage(const Image &img) {
    // Get raw MiDaS depth
    Image depth = computeDepthMap(img);

    // Light smoothing for cleaner visualization (optional but recommended)
    Image smooth = blur(depth, 1.5);

    Image out(img.width(), img.height());

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            float d = smooth.at(x, y).r;

            // Clamp for safety
            d = std::clamp(d, 0.0f, 1.0f);

            float v = d * 255.0f;

            Pixel &p = out.at(x, y);
            p.r = p.g = p.b = v;
        }
    }

    return out;
}