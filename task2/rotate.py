import numpy as np

def get_angle(img):
    def f(img, angle):
        rot = img.rotate(angle, expand=True, fillcolor=(255))
        res = []
        means = np.array(rot).mean(axis=1)
        for i in range(1, len(means)):
            res.append(means[i] - means[i - 1])
        return np.array(res).std()
    
    img_gs = img.convert('L')
    scores = []
    for i in range(-90, 91, 1):
        scores.append(f(img_gs, i))
    
    return np.argmax(scores) - 90

def rotate_img(img):
    return img.rotate(get_angle(img), expand=True, fillcolor=(255))