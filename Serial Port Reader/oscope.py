import argparse
import numpy
from matplotlib import pyplot
from matplotlib import animation

########################################################################
parser = argparse.ArgumentParser(description='Audio visualizer.')
parser.add_argument('--trigger', type=int, default=10,
                    help='Moving average (lowpass) filter audio before finding a rising edge. Set to 0 for no triggering at all')
parser.add_argument('--display_size', type=int, default=256)
parser.add_argument('--no_dc', action='store_true', help='Use this flag to remove any DC offset in the display')
parser.add_argument('--chunk', type=int, default=1024, help='Audio chunk size')
args = parser.parse_args()

stream = audio.audio_input(args.chunk)
"""
def plot_animation(display_size, trigger, no_dc, chunk_size, audio_stream):
    drawer = DrawFunctions(audio_stream, display_size, no_dc, trigger)
    drawer.start_animation()
"""
class DrawFunctions:
    def __init__(self, audio_stream, display_size, no_dc, trigger):
        self.audio_stream = audio_stream
        self.display_size = display_size
        self.no_dc = no_dc
        self.trigger = trigger

        self.fig = pyplot.figure()
        ax = pyplot.axes(xlim=(0, display_size), ylim=(-2**15, 2**15))
        self.line, = ax.plot([], [], lw=2)

    def init(self):
        self.line.set_data([], [])
        return self.line,

    def start_animation(self):
        ani = animation.FuncAnimation(self.fig,
                                      self.update,
                                      init_func=self.init,
                                      interval=33,
                                      blit=True)
        pyplot.show()

    def update(self,i):
        frames = self.audio_stream.read_audio()
        x = numpy.arange(self.display_size)

        if self.trigger:
            center = self.find_center(frames, self.display_size, self.trigger)
        else:
            center = len(frames) / 2

        if self.no_dc:
            avg = sum(frames[center - self.display_size/2 :
                             center + self.display_size/2])
            avg /= self.display_size
            ac_coupled = [i - avg for i in
                          frames[center - self.display_size/2 :
                                 center + self.display_size/2]]
            self.line.set_data(x, ac_coupled)
        else:
            self.line.set_data(x, frames[center - self.display_size/2 :
                                         center + self.display_size/2])
        return self.line,

    def find_center(self, data, display_size, trigger):
        count = 0
        for position in range(display_size, len(data) - display_size):
            if count == 0:
                if data[position] < 0:
                    count = 1
            else:
                if data[position] > 0:
                    return position
                else:
                    count = 0
        return int(len(data) / 2)

    def lowpass(self, data, order):
        filtered = [sum(data[position:position + order]) / order
                    for position in range(len(data) - order)]
        return filtered


#plot_animation(args.display_size, args.trigger, args.no_dc, args.chunk,stream) 