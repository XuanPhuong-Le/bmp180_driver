from flask import Flask, render_template
import subprocess

app = Flask(__name__)


def read_sensor():
    try:
        output = subprocess.check_output(["./bmp180_demo"], universal_newlines=True)
        lines = output.strip().split('\n')
        temp_line = [line for line in lines if "Temperature:" in line][-1]
        press_line = [line for line in lines if "Pressure:" in line][-1]
        alt_line = [line for line in lines if "Altitude:" in line][-1]

        temp = float(temp_line.split(":")[1].strip().split()[0])
        press = float(press_line.split(":")[1].strip().split()[0]) * 100
        alt = float(alt_line.split(":")[1].strip().split()[0])

        forecast = "Không mưa" if press > 100000 else "Có khả năng mưa"
        advice = "Thời tiết đẹp, hãy tận hưởng ngày của bạn!" if forecast == "Không mưa" else "Dự báo có mưa, bạn nên mang ô khi ra ngoài."

        return {
            "temperature": f"{temp:.1f} °C",
            "pressure": f"{press:.0f} Pa",
            "altitude": f"{alt:.1f} m",
            "forecast": forecast,
            "advice": advice
        }
    except Exception as e:
        return {
            "temperature": "-- °C",
            "pressure": "-- Pa",
            "altitude": "-- m",
            "forecast": "Không xác định",
            "advice": f"Lỗi: {str(e)}"
        }

@app.route('/')
def index():
    data = read_sensor()
    return render_template('index.html', data=data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
