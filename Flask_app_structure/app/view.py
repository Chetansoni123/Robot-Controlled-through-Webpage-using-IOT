from app import app
import requests

from flask import render_template, redirect, url_for


@app.route('/')
def home():
	return render_template("bot.html")


# @app.route('/orange')
# def orange():
# 	URL = "http://192.168.43.10:80/?robot=FWD"
# 	r = requests.get(url = URL)
# 	return redirect(url_for("home"))


# @app.route('/white')
# def white():
# 	URL = "http://192.168.43.10:80/?robot=REV"
# 	r = requests.get(url = URL)
# 	return redirect(url_for("home"))


# @app.route('/green')
# def green():
# 	URL = "http://192.168.43.10:80/?robot=RGT"
# 	r = requests.get(url = URL)
# 	return redirect(url_for("home"))


# @app.route('/blue')
# def blue():
# 	URL = "http://192.168.43.10:80/?robot=LFT"
# 	r = requests.get(url = URL)
# 	return redirect(url_for("home"))


# @app.route('/StopAll')
# def StopAll():
# 	URL = "http://192.168.43.10:80/?robot=STP"
# 	r = requests.get(url = URL)
# 	return redirect(url_for("home"))

	


