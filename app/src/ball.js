function encodeSpinAngle(angle) {
  return ((parseInt(angle, 10) + 360) % 360) / 2;
}

function decodeSpinAngle(value) {
  let result = value * 2;
  if (result <= 180) {
    return result;
  }

  return result - 360;
}

class Ball {
  constructor(spin_angle, spin_strength, trajectory, position, speed) {
    this.spin_angle = spin_angle;
    this.spin_strength = spin_strength;
    this.trajectory = trajectory;
    this.position = position;
    this.speed = speed;
  }

  copy() {
    return new Ball(
      this.spin_angle, this.spin_strength, this.trajectory, this.position,
      this.speed,
    );
  }

  static decode(buffer) {
    let view = new DataView(buffer);
    return new Ball(
        decodeSpinAngle(view.getUint8(0)),
        view.getUint8(1),
        view.getUint8(2),
        view.getInt8(3),
        view.getUint8(4));
  }

  static get encodedSize() {
    return 5;
  }

  static encode(ball, result) {
    result[0] = encodeSpinAngle(ball.spin_angle);
    result[1] = ball.spin_strength;
    result[2] = ball.trajectory;
    result[3] = ball.position;
    result[4] = ball.speed;

    return result.buffer;
  }
}

export default Ball;
